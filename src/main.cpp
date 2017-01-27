#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <regex>
#include <exception>
#include <math.h>
#include <float.h>
#include <list>
#include "util.hpp"
#include "DataLine.h"
#include "ShiftedCoordinateGrid.h"


int data_point_count(std::string);
std::vector<std::vector<float>> read_data(std::string);
std::vector<sf::VertexArray> draw_line_graph(std::vector<std::vector<float>>);
std::vector<sf::VertexArray> draw_scaled_graph(std::vector<std::vector<float>>);
sf::Vector2f get_largest_smallest_data_points(std::vector<std::vector<float>> data);
sf::Vector2f get_largest_smallest_data_points_at_index(std::vector<std::vector<float>> data, int index);
void draw_graph_background(int segment_count, sf::RenderWindow *window);
void normalize_data(std::vector<std::vector<float>> *data);

float scale_between(float input, float upper_scale, float lower_scale, float upper_bound, float lower_bound) {
	return (upper_scale - lower_scale) * (input - lower_bound) / (upper_bound - lower_bound) + lower_scale;
}


int main() {

	sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE_X, WINDOW_SIZE_Y), "cs445");
	sf::FloatRect viewport_rect(0, 0, 800, 800);
	sf::View viewport(viewport_rect);
	window.setView(viewport);

	sf::Font f;
	f.loadFromFile("../assets/code_bold.otf");

	sf::Text y_max_text;
	y_max_text.setFont(f);
	y_max_text.setString("string");
	y_max_text.setCharacterSize(14);
	y_max_text.setColor(sf::Color::Black);
	y_max_text.setPosition(sf::Vector2f(10, 10));
	
	
	
	
	// This is where the data is read, change the path for another file
	std::vector<std::vector<float>> raw_data = read_data("../data/doubled_data.txt");
	//normalize_data(&raw_data);


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
	for (int i = 0; i < raw_data.at(0).size() / 2; i++) {
		//                       + a lil padding
		int x_pos = step * i * 2 + step / 2;
		coordinates.push_back(ShiftedCoordinateGrid(sf::Vector2f(x_pos, 400)));
	}

	
	

	enum Mouse_State { PRESSED, DEPRESSED };
	Mouse_State mouse_state = Mouse_State::DEPRESSED;

	sf::Vector2i last_mouse_position = sf::Mouse::getPosition();

	int zoom_level = 50;

	enum Graph_Type { A, B, C, D, E, F, G, H };
	Graph_Type graph_type = A;
	
	while (window.isOpen()) {

		sf::Event event;
		while (window.pollEvent(event)) {

			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (event.type == sf::Event::KeyPressed){
				if (event.key.code == sf::Keyboard::A){
					data_lines.at(0).shift_coords_to_match(&coordinates);
				}
				if (event.key.code == sf::Keyboard::B) {
					coordinates.at(1).flip_bit();
				}
				if (event.key.code == sf::Keyboard::C) {
					coordinates.at(2).flip_bit();
				}
				if (event.key.code == sf::Keyboard::D) {
					data_lines.at(1).shift_coords_to_match(&coordinates);
				}
				if (event.key.code == sf::Keyboard::E) {
					coordinates.at(0).collapse_to_point(coordinates.at(1), data_lines.at(1), 0, 1);
				}
				if (event.key.code == sf::Keyboard::F) {

				}
				if (event.key.code == sf::Keyboard::G) {

				}
				if (event.key.code == sf::Keyboard::H) {

				}
				if (event.key.code == sf::Keyboard::R) {

				}
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

		for (int i = 0; i < coordinates.size(); i++) {
			coordinates.at(i).draw(&window);
			coordinates.at(i).draw_text(&window, i*2);
		}

		for (auto i : data_lines)
			i.draw(coordinates, &window);

		window.draw(y_max_text);
		
		window.display();

	}

	return 1;
}


std::vector<sf::VertexArray> draw_scaled_graph(std::vector<std::vector<float>> data) {

	// Sort the vector by the first element
	std::sort(data.begin(), data.end(), [](const std::vector< float >& a, const std::vector< float >& b){ return a[0] > b[0]; } );

	int red_index = data.size() / 2;
	std::vector<float> red_index_y = data.at(red_index);

	// Get the datas highest and lowest values to scale with
	// Set the line to a random
	sf::Vector2f bounds = get_largest_smallest_data_points(data);

	// The x increment that we step by
	int step = WINDOW_SIZE_X / data.at(0).size();

	std::vector<sf::VertexArray> vertex_array_array;

	for (int x = 0; x < data.size(); x++) {

		int x_pos = 0;

		sf::VertexArray v;
		v.setPrimitiveType(sf::LinesStrip);

		for (int y = 0; y < data.at(x).size(); y++) {

			float y_pos = (data.at(x).at(y) + (red_index_y.at(0) - red_index_y.at(y))) * ((WINDOW_SIZE_Y / bounds.y ) / 2) + WINDOW_SIZE_Y/4;
			sf::Vertex v1(sf::Vector2f(x_pos, y_pos));

			if (x == red_index)
				v1.color = sf::Color(0, 0, 0);
			else
				v1.color = sf::Color(0, 255, 0);

			v.append(v1);

			x_pos += step;
		}

		vertex_array_array.push_back(v);
	}

	return vertex_array_array;

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

void draw_graph_background(int segment_count, sf::RenderWindow *window) {
	
	// The x increment that we step by
	int step = WINDOW_SIZE_X / segment_count;

	int pos = 0;

	while (pos < WINDOW_SIZE_X) {
		
		sf::RectangleShape line(sf::Vector2f(2, WINDOW_SIZE_Y));
		line.setPosition(sf::Vector2f(pos, 0));
		line.setFillColor(sf::Color::Black);
		window->draw(line);

		pos += step;
	}
	
}

// This function takes the 2d array of data and returns multiple vertex arrays to draw
std::vector<sf::VertexArray> draw_line_graph(std::vector<std::vector<float>> d) {
	
	// Get the datas highest and lowest values to scale with
	sf::Vector2f bounds = get_largest_smallest_data_points(d);

	// The x increment that we step by
	int step = WINDOW_SIZE_X / d.at(0).size();

	std::vector<sf::VertexArray> vertex_array_array;

	for (int x = 0; x < d.size(); x++) {

		int x_pos = 0;

		sf::VertexArray v;
		v.setPrimitiveType(sf::LinesStrip);
		
		// Set the line to a random color		
		sf::Color color(rand() % 255, rand() % 255, rand() % 255);

		for (int y = 0; y < d.at(x).size(); y++) {

			sf::Vertex v1(sf::Vector2f(x_pos, WINDOW_SIZE_Y - d.at(x).at(y) * (WINDOW_SIZE_Y / bounds.y ) / 2 - WINDOW_SIZE_Y/4));
			v1.color = color;

			v.append(v1);

			x_pos += step;
		}

		vertex_array_array.push_back(v);
	}

	return vertex_array_array;

}



// This function reads in a file line by line
// Each line is tokenized by spaces, commas, or by quoted strings
// The tokens are then converted to floats and added to an array
// Once a line has been read and put into the vector, that vector is then added to another vector for storage
// When done, the 2d vector is then returned

std::vector<std::vector<float>> read_data(std::string file_path) {

	std::vector<std::vector<float>> data;

	std::ifstream stream;
	stream.open(file_path);

	if (!stream) {
		return data;
	}

	std::string line;

	while (std::getline(stream, line)) {

		std::regex reg("\,\\s*");
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
