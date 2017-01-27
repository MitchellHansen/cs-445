#pragma once
#include <SFML/Graphics/Vertex.hpp>
#include <fstream>
#include "DataLine.h"
#include <iostream>
#include "ShiftedCoordinateGrid.h"



DataLine::DataLine(std::vector<float> normalized_data, int data_class, std::vector<ShiftedCoordinateGrid> *coordinate_grids) :
	data(normalized_data), data_class(data_class), coordinate_grids(coordinate_grids) {

	if (data_class == 1)
		color = sf::Color::Blue;
	else if (data_class == 2)
		color = sf::Color::Green;
	else if (data_class == 3)
		color = sf::Color::Red;
	else
		color = sf::Color::Yellow;



	for (int i = 0; i < data.size() - 1; i += 2) {
		doubled_coords.push_back(sf::Vector2f(data.at(i), data.at(i + 1)));
	}

	if (data.size() % 2 == 1)
		doubled_coords.push_back(sf::Vector2f(data.back(), data.back()));

}

std::vector<sf::Vector2f> DataLine::get_doubled_coords() {
	return doubled_coords;
}


std::vector<sf::Vector2f> DataLine::get_doubled_shifts() {
	return doubled_shifts;
}

sf::Vector2f DataLine::bounds() {

	float largest = FLT_MAX;
	float smallest = FLT_MIN;

	for (auto i : data) {

		if (i > largest)
			largest = i;

		if (i < smallest)
			largest = i;
	}

	return sf::Vector2f(largest, smallest);
}


// Shift the data line in coordinate grid space to match the first element
void DataLine::shift_coords_to_match(std::vector<ShiftedCoordinateGrid> *coords) {

	std::vector<sf::Vector2f> paired_coordinates = get_doubled_coords();

	// Scale the values to pixel space
	//for (int i = 0; i < paired_coordinates.size(); i++)
	//	paired_coordinates.at(i) *= SCALE_VALUE;

	if (coords->size() < paired_coordinates.size()) {
		std::cout << "not enough axes for the amount of points";
		return;
	}

	for (int i = 1; i < paired_coordinates.size(); i++) {

		sf::Vector2f shift =
			coordinate_grids->at(0).shift_point_in_normalized_space(paired_coordinates.at(0)) -
			coordinate_grids->at(i).shift_point_in_normalized_space(paired_coordinates.at(i));
		shift.x = 0;

		coords->at(i).shift_coordinate(shift);

	}

}

void DataLine::draw(std::vector<AxisLine> lines, sf::RenderWindow *window) {





}

void DataLine::draw(std::vector<ShiftedCoordinateGrid> coordinates, sf::RenderWindow *window) {

	// Get the coordinate pairs and check the data
	std::vector<sf::Vector2f> paired_coordinates = get_doubled_coords();

	if (coordinates.size() < paired_coordinates.size()) {
		std::cout << "not enough axes for the amount of points";
		return;
	}

	sf::VertexArray vertex_array;
	vertex_array.setPrimitiveType(sf::LinesStrip);

	// For every set of pair coordinates
	for (int i = 0; i < paired_coordinates.size(); i++) {

		// Get the coordinate grid that it'll be in.
		// Then, give the grid the paired normalized coords.
		// It will then return the screen space position of that point.
		
		sf::Vector2f point = coordinates.at(i).shift_point(paired_coordinates.at(i));

		// Flip the Y axis
		point.y = window->getSize().y - point.y;

		// Set the vertex, and its color
		sf::Vertex vertex(point);
		vertex.color = color;

		vertex_array.append(vertex);
	}

	window->draw(vertex_array);
}
