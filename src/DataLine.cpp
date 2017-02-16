
#include "DataLine.h"

DataLine::DataLine(std::vector<float> normalized_data, int data_class) :
	data(normalized_data), data_class(data_class) {

	if (data_class == 1)
		color = sf::Color::Blue;
	else if (data_class == 2)
		color = sf::Color::Green;
	else if (data_class == 3)
		color = sf::Color::Red;
	else
		color = sf::Color::Yellow;
}

std::vector<sf::Vector2f> DataLine::doubled_coords() {

	std::vector<sf::Vector2f> out;

	for (int i = 0; i < data.size() - 1; i += 2) {
		out.push_back(sf::Vector2f(data.at(i), data.at(i + 1)));
	}

	if (data.size() % 2 == 1)
		out.push_back(sf::Vector2f(data.back(), data.back()));

	return out;

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

	std::vector<sf::Vector2f> paired_coordinates = doubled_coords();

	// Scale the values to pixel space
	for (int i = 0; i < paired_coordinates.size(); i++)
		paired_coordinates.at(i) *= SCALE_VALUE;

	if (coords->size() < paired_coordinates.size()) {
		std::cout << "not enough axes for the amount of points";
		return;
	}

	for (int i = 1; i < paired_coordinates.size(); i++) {

		sf::Vector2f shift = paired_coordinates.at(0) - paired_coordinates.at(i);
		shift.x = 0;

		coords->at(i).shift_coordinate_pixel_space(shift);

	}
}

void DataLine::shift_axis_lines_to_point(std::vector<AxisLine> *lines, int point){

    float point_pos_on_line = data.at(point);

    sf::Vector2f origin = lines->at(point).get_point(point_pos_on_line);

    for (int i = 0; i < lines->size(); i++){
        sf::Vector2f line_origin = lines->at(i).get_point(data.at(i));
        lines->at(i).shift_line(origin - line_origin);
    }

}

void DataLine::draw(std::vector<AxisLine> lines, sf::RenderWindow *window) {

    sf::VertexArray vertex_array;
    vertex_array.setPrimitiveType(sf::LinesStrip);

    for (int i = 0; i < data.size(); i++){

        sf::Vector2f point = lines.at(i).get_point(data.at(i));

        //point.y = window->getSize().y - point.y;

        sf::Vertex v1(point);
        v1.color = color;

        vertex_array.append(v1);

    }

    window->draw(vertex_array);

}

void DataLine::draw_bezier(std::vector<AxisLine> lines, sf::RenderWindow *window) {

    for (int i = 0; i < data.size(); i++){


        sf::Vector2f point = lines.at(i).get_point(data.at(i));

        if (i + 1 < data.size()) {

            sf::Vector2f midpoint = lines.at(i).get_midpoint(data.at(i), lines.at(i+1), data.at(i+1));

            if (data_class == 1)
                midpoint.x = (point.x - midpoint.x) + point.x;

            sf::VertexArray vertices(sf::LinesStrip, 0);

            sf::Vector2f p1 = point;
            sf::Vector2f p2 = lines.at(i+1).get_point(data.at(i+1));
            sf::Vector2f p3 = midpoint;

            std::vector<sf::Vector2f> points =
                    CalcCubicBezier(
                            point,
                            lines.at(i+1).get_point(data.at(i+1)),
                            midpoint,
                            midpoint,
                            10
                    );

            for (std::vector<sf::Vector2f>::const_iterator a = points.begin(); a != points.end(); ++a)
                vertices.append(sf::Vertex(*a, color));

            window->draw(vertices);

        }
    }

}

void DataLine::draw(std::vector<BezierAxisLine> lines, sf::RenderWindow *window) {

    sf::VertexArray vertex_array;
    vertex_array.setPrimitiveType(sf::LinesStrip);

    for (int i = 0; i < data.size(); i++){

        int index = lines.at(i).get_index();
        sf::Vector2f point = lines.at(i).get_point(data.at(index));

        sf::Vertex v1(point);
        v1.color = color;

        vertex_array.append(v1);

        if (i + 1 < data.size()) {
            sf::Vector2f midpoint = lines.at(i).get_midpoint(data.at(i), lines.at(i+1), data.at(i+1));


            if (data_class == 1)
                midpoint.y = (lines.at(i).head.y - midpoint.y) + lines.at(i).head.y;
            sf::Vertex v2(midpoint);
            v2.color = color;
            vertex_array.append(v2);

            sf::VertexArray vertices(sf::LinesStrip, 0);

            std::vector<sf::Vector2f> points =
                    CalcCubicBezier(
                            point,
                            lines.at(i+1).get_point(data.at(index)),
                            midpoint,
                            midpoint,
                            10);

            for (std::vector<sf::Vector2f>::const_iterator a = points.begin(); a != points.end(); ++a)
                vertices.append(sf::Vertex(*a, sf::Color::Magenta));

            window->draw(vertices);

        }
    }

    window->draw(vertex_array);

}

void DataLine::draw_bezier(std::vector<BezierAxisLine> lines, sf::RenderWindow *window) {

    for (int i = 0; i < data.size(); i++){

        int index = lines.at(i).get_index();
        sf::Vector2f point = lines.at(i).get_point(data.at(index));

        if (i + 1 < data.size()) {

            sf::Vector2f midpoint = lines.at(i).get_midpoint(data.at(i), lines.at(i+1), data.at(i+1));

            if (data_class == 1)
                midpoint.y = (lines.at(i).head.y - midpoint.y) + lines.at(i).head.y;

            sf::VertexArray vertices(sf::LinesStrip, 0);

            std::vector<sf::Vector2f> points =
                    CalcCubicBezier(
                            point,
                            lines.at(i+1).get_point(data.at(index)),
                            midpoint,
                            midpoint,
                            10);

            for (std::vector<sf::Vector2f>::const_iterator a = points.begin(); a != points.end(); ++a)
                vertices.append(sf::Vertex(*a, color));

            window->draw(vertices);

        }
    }

}



void DataLine::draw(std::vector<ShiftedCoordinateGrid> coordinates, sf::RenderWindow *window) {

	// Get the coordinate pairs and check the data
	std::vector<sf::Vector2f> paired_coordinates = doubled_coords();

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

std::vector<int> DataLine::reorder() {

    std::vector<int> idx(data.size());
    std::iota(idx.begin(), idx.end(), 0);

    std::sort(idx.begin(), idx.end(),
         [&](size_t i1, size_t i2) {return data[i1] < data[i2];});

    return idx;
}
