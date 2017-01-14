#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <regex>
#include <exception>
#include <math.h>
#include <float.h>

const int WINDOW_SIZE_X = 800;
const int WINDOW_SIZE_Y = 800;

int data_point_count(std::string);
std::vector<std::vector<float>> read_data(std::string);
void draw_line_graph(std::vector<std::vector<float>>, sf::RenderWindow *window);

int main() {

	sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE_X, WINDOW_SIZE_Y), "cs445");

	std::vector<std::vector<float>> data = read_data("../data/car_mpg.txt");

	while (window.isOpen()) {

		sf::Event event;
		while (window.pollEvent(event)) {

			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		window.clear(sf::Color(255, 255, 255));
		
		draw_line_graph(data, &window);


		window.display();

	}

	return 1;
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

void draw_line_graph(std::vector<std::vector<float>> d, sf::RenderWindow *window) {
	
	sf::Vector2f bounds = get_largest_smallest_data_points(d);

	sf::VertexArray v;

	v.setPrimitiveType(sf::LinesStrip);
	

	int step = WINDOW_SIZE_X / d.at(0).size();

	for (int x = 0; x < d.size(); x++) {

		int x_pos = 0;
		//sf::Color c(rand()%255, rand()%255, rand()%255);

		for (int y = 0; y < d.at(x).size(); y++) {

			sf::Vertex v1(sf::Vector2f(x_pos, WINDOW_SIZE_Y - d.at(x).at(y) * (WINDOW_SIZE_Y / bounds.y )));
			//sf::Vertex v2(sf::Vector2f(x_pos, WINDOW_SIZE_Y - d.at(y).at(x) * (WINDOW_SIZE_Y / bounds.y )));
			v1.color = sf::Color::Blue;
			//v2.color = c;
			v.append(v1);

			x_pos += step;
		}

		window->draw(v);
		v.clear();
	}

}


std::vector<std::vector<float>> read_data(std::string file_path) {
	
	std::vector<std::vector<float>> data;

	std::ifstream stream;
	stream.open(file_path);

	if (!stream){
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
			} catch (std::invalid_argument){
				entry.push_back(0.0f);
			}
			
			iterator++;
		}


		data.push_back(entry);
	}

	return data;

}
