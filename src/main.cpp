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
std::vector<sf::VertexArray> draw_line_graph(std::vector<std::vector<float>>);
sf::Vector2f get_largest_smallest_data_points(std::vector<std::vector<float>> data);
sf::Vector2f get_largest_smallest_data_points_at_index(std::vector<std::vector<float>> data, int index);
void draw_graph_background(int segment_count, sf::RenderWindow *window);
void normalize_data(std::vector<std::vector<float>> *data);
float scale_between(float input, float upper_scale, float lower_scale, float upper_bound, float lower_bound) {
	return (upper_scale - lower_scale) * (input - lower_bound) / (upper_bound - lower_bound) + lower_scale;
}

int main() {

	sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE_X, WINDOW_SIZE_Y), "cs445");


	// This is where the data is read, change the path for another file
	std::vector<std::vector<float>> data = read_data("../data/sts.txt");
	normalize_data(&data);
	std::vector<sf::Color> sort_and_assign_colors;

	sf::Font f;
	f.loadFromFile("../assets/code_bold.otf");

	sf::Text y_max_text;
	y_max_text.setFont(f);
	y_max_text.setString(std::to_string((int)get_largest_smallest_data_points(data).y));
	y_max_text.setCharacterSize(14);
	y_max_text.setColor(sf::Color::Black);
	y_max_text.setPosition(sf::Vector2f(10, 10));

	sf::Text x_max_text;
	x_max_text.setFont(f);
	x_max_text.setString(std::to_string(data.at(0).size()));
	x_max_text.setCharacterSize(14);
	x_max_text.setColor(sf::Color::Black);
	x_max_text.setPosition(sf::Vector2f(WINDOW_SIZE_X - 30, WINDOW_SIZE_Y - 30));


	std::vector<sf::VertexArray> lines = draw_line_graph(data);

	while (window.isOpen()) {

		sf::Event event;
		while (window.pollEvent(event)) {

			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		window.clear(sf::Color(255, 255, 255));
		
		draw_graph_background(data.at(0).size(), &window);

		for (auto i : lines)
			window.draw(i);

		window.draw(y_max_text);
		window.draw(x_max_text);
		window.display();

	}

	return 1;
}

void normalize_data(std::vector<std::vector<float>> *data) {

	for (int dimension = 0; dimension < data->at(0).size(); dimension++) {

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
