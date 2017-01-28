#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "AxisLine.h"

class ShiftedCoordinateGrid;

class DataLine {

public:

	DataLine(std::vector<float> normalized_data, int data_class);

	// The coords put into (x1,x2)(x3,x4) pairs
	std::vector<sf::Vector2f> doubled_coords();

	// Upper and lower bounds to the line
	sf::Vector2f bounds();

	void shift_coords_to_match(std::vector<ShiftedCoordinateGrid> *coords);
    void shift_axis_lines_to_point(std::vector<AxisLine> *lines, int point);

	void draw(std::vector<AxisLine> lines, sf::RenderWindow *window);
	void draw(std::vector<ShiftedCoordinateGrid> coordinates, sf::RenderWindow *window);

private:

	std::vector<float> data;
	int data_class = 0;

	sf::Color color;

protected:

};
