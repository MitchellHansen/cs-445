#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "AxisLine.h"
#include "BezierAxisLine.h"
#include <SFML/Graphics/Vertex.hpp>
#include <fstream>
#include <iostream>
#include "ShiftedCoordinateGrid.h"
#include <float.h>

class ShiftedCoordinateGrid;
class AxisLine;

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
    void draw_bezier(std::vector<AxisLine> lines, sf::RenderWindow *window);
	void draw(std::vector<ShiftedCoordinateGrid> coordinates, sf::RenderWindow *window);
    void draw(std::vector<BezierAxisLine> coordinates, sf::RenderWindow *window);
    void draw_bezier(std::vector<BezierAxisLine> lines, sf::RenderWindow *window);
	void draw_lab7(std::vector<float> scalar, sf::RenderWindow *window);

    std::vector<int> reorder();

private:

	std::vector<float> data;
	int data_class = 0;

	sf::Color color;

protected:

};
