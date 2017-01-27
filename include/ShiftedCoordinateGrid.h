#pragma once
#include <SFML/Graphics.hpp>

class DataLine;

class ShiftedCoordinateGrid {
public:
	ShiftedCoordinateGrid(sf::Vector2f origin);

	void flip_bit();

	// Receive a point in [0-1] coords
	sf::Vector2f shift_point(sf::Vector2f point);

	// Give the coordinate grid a [0-1] normalized coord to shift to 
	// Used for straightening lines, where you are calculating line heights in normalized coords
	void shift_coordinate(sf::Vector2f shift);
	void shift_coordinate_pixel_space(sf::Vector2f shift);

	// Receive the grid to move to, and the line which we are following
	void collapse_to_point(ShiftedCoordinateGrid neighbor_grid, DataLine line, int index1, int index2);

	void draw(sf::RenderWindow *window);
	void draw_text(sf::RenderWindow *window, int index);

	sf::Vector2f get_origin();
	sf::Vector2f get_shift();

private:
	sf::Vector2f origin;
	sf::Vector2f origin_shift;
	bool flipped = false;

	sf::Font f;
	sf::Text x1x2_text;



};
