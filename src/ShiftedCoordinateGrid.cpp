#pragma once
#include <SFML/Graphics.hpp>
#include "ShiftedCoordinateGrid.h"
#include "DataLine.h"

ShiftedCoordinateGrid::ShiftedCoordinateGrid(sf::Vector2f origin) : origin(origin) {
	f.loadFromFile("../assets/code_bold.otf");
	x1x2_text.setFont(f);
	x1x2_text.setString("");
	x1x2_text.setCharacterSize(8);
	x1x2_text.setColor(sf::Color::Black);

};

void ShiftedCoordinateGrid::flip_bit() {
	flipped = !flipped;
}

// Take normalized line point and return shifted point
sf::Vector2f ShiftedCoordinateGrid::shift_point(sf::Vector2f point) {

	// Convert the normalized coords to pixels
	point *= SCALE_VALUE;

	// If the point was flipped to do that cross shit
	if (flipped) {

		point.y = SCALE_VALUE - point.y;
		
		// return the pixel space point, added to the origin and it's shift
		return (point + origin + origin_shift);
	}

	// Else 
	else
		return point + origin + origin_shift;
}

void ShiftedCoordinateGrid::shift_coordinate(sf::Vector2f shift) {
	this->origin_shift = shift * SCALE_VALUE;
}

void ShiftedCoordinateGrid::shift_coordinate_pixel_space(sf::Vector2f shift) {
	this->origin_shift = shift;
}

void ShiftedCoordinateGrid::collapse_to_point(ShiftedCoordinateGrid neighbor_grid, DataLine line, int index1, int index2) {

	// Move the grid to the neighbors grid
	origin = neighbor_grid.get_origin();
	
	std::vector<sf::Vector2f> coords = line.doubled_coords();
	sf::Vector2f this_coord = coords.at(index1);
	sf::Vector2f next_coord = coords.at(index2);
	
	sf::Vector2f difference = next_coord - this_coord;
	
	origin_shift += difference;
	
}

void ShiftedCoordinateGrid::draw(sf::RenderWindow *window) {

	sf::VertexArray vertex_array;
	vertex_array.setPrimitiveType(sf::LinesStrip);

	sf::Vertex v1(origin + origin_shift, sf::Color::Black);
	sf::Vertex vTop(origin + origin_shift + sf::Vector2f(0, 80), sf::Color::Black);
	sf::Vertex vRight(origin + origin_shift + sf::Vector2f(80, 0), sf::Color::Black);

	// Flip the Y axis
	v1.position.y = window->getSize().y - v1.position.y;
	vTop.position.y = window->getSize().y - vTop.position.y;
	vRight.position.y = window->getSize().y - vRight.position.y;

	vertex_array.append(vTop);
	vertex_array.append(v1);
	vertex_array.append(vRight);

	window->draw(vertex_array);

}

void ShiftedCoordinateGrid::draw_text(sf::RenderWindow *window, int index) {

	x1x2_text.setFont(f);

	x1x2_text.setPosition(origin + -origin_shift + sf::Vector2f(0, -80));
	x1x2_text.setString("x" + std::to_string(index + 1));
	window->draw(x1x2_text);

	x1x2_text.setPosition(origin + -origin_shift + sf::Vector2f(80, 0));
	x1x2_text.setString("x" + std::to_string(index));
	window->draw(x1x2_text);
}

sf::Vector2f ShiftedCoordinateGrid::get_origin() {
	return origin;
}

sf::Vector2f ShiftedCoordinateGrid::get_shift() {
	return origin_shift;
}
