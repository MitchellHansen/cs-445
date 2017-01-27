#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "AxisLine.h"

class ShiftedCoordinateGrid;

class DataLine {

public:

	DataLine(std::vector<float> normalized_data, int data_class, std::vector<ShiftedCoordinateGrid> *coordinate_grids);

	// The coords put into (x1,x2)(x3,x4) pairs
	std::vector<sf::Vector2f> get_doubled_coords();
	std::vector<sf::Vector2f> get_doubled_shifts();

	// Upper and lower bounds to the line
	sf::Vector2f bounds();

	void shift_coords_to_match(std::vector<ShiftedCoordinateGrid> *coords);

	void draw(std::vector<AxisLine> lines, sf::RenderWindow *window);
	void draw(std::vector<ShiftedCoordinateGrid> coordinates, sf::RenderWindow *window);

private:
	
	std::vector<sf::Vector2f> doubled_coords;
	std::vector<sf::Vector2f> doubled_shifts;
	
	std::vector<float> data;
	int data_class = 0;

	sf::Color color;

	std::vector<ShiftedCoordinateGrid> *coordinate_grids;

protected:

};
