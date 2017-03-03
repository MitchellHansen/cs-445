#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <fstream>
#include <regex>
#include <exception>
#include <math.h>
#include <float.h>
#include <list>
#include <BezierAxisLine.h>
#include "util.hpp"
#include "DataLine.h"
#include "ShiftedCoordinateGrid.h"


std::vector<std::vector<float>> read_data(std::string);
sf::Vector2f get_largest_smallest_data_points(std::vector<std::vector<float>> data);
sf::Vector2f get_largest_smallest_data_points_at_index(std::vector<std::vector<float>> data, int index);
void normalize_data(std::vector<std::vector<float>> *data);
std::vector<std::vector<float>> cull_data_within_t(std::vector<std::vector<float>> *data);
std::vector<std::vector<float>> non_culled_data(std::vector<std::vector<float>> *data);

float scale_between(float input, float upper_scale, float lower_scale, float upper_bound, float lower_bound) {
	return (upper_scale - lower_scale) * (input - lower_bound) / (upper_bound - lower_bound) + lower_scale;
}

enum Mouse_State { PRESSED, DEPRESSED };

int main() {


	sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE_X, WINDOW_SIZE_Y), "cs445");
	sf::FloatRect viewport_rect(0, 0, 800, 800);
	sf::View viewport(viewport_rect);
	window.setView(viewport);

	sf::Font f;
	f.loadFromFile("../assets/code_bold.otf");

	sf::Text y_max_text;
	y_max_text.setFont(f);
	y_max_text.setString("Visual Analytics\nMitchell Hansen\nFrank Senseney\nMatthew Stalder");
	y_max_text.setCharacterSize(12);
	y_max_text.setColor(sf::Color::Black);
	y_max_text.setPosition(sf::Vector2f(10, 10));
	
	
	// This is where the data is read, change the path for another file
	std::vector<std::vector<float>> raw_data = read_data("../data/glass.txt");

    normalize_data(&raw_data);


    std::vector<float> scalar(raw_data.at(0).size(), 1);
    for (int i = 0; i < scalar.size(); i++){
        scalar.at(i) = (rand() % 3) + 1;
    }

    std::vector<DataLine> data_lines;

	// Initialize the container for the lines of data
	for (int line = 0; line < raw_data.size(); line++) {
		
		std::vector<float> raw_line_data = raw_data.at(line);

		int data_class = raw_line_data.back();
		raw_line_data.pop_back();

		data_lines.push_back(DataLine(raw_line_data, data_class));
	}

	// Initialize the paired coordinates
	std::vector<ShiftedCoordinateGrid> coordinates;

	int step = WINDOW_SIZE_X / raw_data.at(0).size();

    // Initialize the axis coordinates
    std::vector<AxisLine> axis_lines;

    // Initialize the bezier coords
    std::vector<BezierAxisLine> bezier_axis_lines;



	Mouse_State mouse_state = Mouse_State::DEPRESSED;

	sf::Vector2i last_mouse_position = sf::Mouse::getPosition();

	int zoom_level = 50;
	
	while (window.isOpen()) {

		sf::Event event;
		while (window.pollEvent(event)) {

			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (event.type == sf::Event::KeyPressed) {

            }
			if (event.type == sf::Event::MouseButtonPressed) {
				mouse_state = Mouse_State::PRESSED;
				
				last_mouse_position = sf::Mouse::getPosition();
			}

			if (event.type == sf::Event::MouseButtonReleased) {
				mouse_state = Mouse_State::DEPRESSED;
			}

			if (event.type == sf::Event::MouseMoved) {
				
				if (mouse_state == Mouse_State::PRESSED) {					
					

					sf::Vector2f delta = static_cast<sf::Vector2f>(last_mouse_position - sf::Mouse::getPosition());
					delta *= ((zoom_level / 100.0f));

					last_mouse_position = sf::Mouse::getPosition();
					
					sf::Vector2f center(viewport.getCenter().x, viewport.getCenter().y);
					center.x += delta.x;
					center.y += delta.y;
					viewport.setCenter(center);

					window.setView(viewport);
				}
			}

			if (event.type == sf::Event::MouseWheelScrolled) {
				zoom_level += event.mouseWheelScroll.delta;
				viewport.setSize((zoom_level / 100.0f) * 1600, (zoom_level / 100.0f) * 1600);
				window.setView(viewport);
			}

		}

		window.clear(sf::Color(255, 255, 255));

//        for (auto i: coordinates) {
//            i.draw(&window);
//        }


        for (auto i: data_lines)
            i.draw_lab7(scalar, &window);

		window.draw(y_max_text);
		
		window.display();

	}

	return 1;
}

void normalize_data(std::vector<std::vector<float>> *data) {

	for (int dimension = 0; dimension < data->at(0).size() - 1; dimension++) {

		sf::Vector2f bounds = get_largest_smallest_data_points_at_index(*data, dimension);

		for (int individual = 0; individual < data->size(); individual++) {

			data->at(individual).at(dimension) = scale_between(data->at(individual).at(dimension), 1, 0, bounds.x, bounds.y);

		}
	}
}

sf::Vector2f get_largest_smallest_data_points(std::vector<std::vector<float>> data){

	float smallest = FLT_MAX;
	float largest = 0;

	for (int x = 0; x < data.at(0).size(); x++) {
		for (int y = 0; y < data.size(); y++) {

			if (data.at(y).at(x) < smallest)
				smallest = data.at(y).at(x);

			if (data.at(y).at(x) > largest)
				largest = data.at(y).at(x);
		}
	}

	return sf::Vector2f(smallest, largest);
}

sf::Vector2f get_largest_smallest_data_points_at_index(std::vector<std::vector<float>> data, int index) {

	float smallest = FLT_MAX;
	float largest = 0;

	
	for (int y = 0; y < data.size(); y++) {

		if (data.at(y).at(index) < smallest)
			smallest = data.at(y).at(index);

		if (data.at(y).at(index) > largest)
			largest = data.at(y).at(index);

	}

	return sf::Vector2f(smallest, largest);
}


std::vector<std::vector<float>> read_data(std::string file_path) {

	std::vector<std::vector<float>> data;

	std::ifstream stream;
	stream.open(file_path);

	if (!stream) {
		return data;
	}

	std::string line;

	while (std::getline(stream, line)) {

		std::regex reg("\\s+|\"(.*?)\"|\\,");
		std::regex_token_iterator<std::string::iterator> iterator(line.begin(), line.end(), reg, -1);
		std::regex_token_iterator<std::string::iterator> end;

		std::vector<float> entry;

		while (iterator != end) {

			try {
				entry.push_back(std::stof(*iterator));
			}
			catch (std::invalid_argument) {
				entry.push_back(0.0f);
			}

			iterator++;
		}


		data.push_back(entry);
	}

	return data;

}
