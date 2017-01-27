#include <SFML/Graphics.hpp>

sf::Vector2f Scale(sf::Vector2f in) {
	return sf::Vector2f(in.x, in.y * 1.0f);
}
float GetMagnitude(sf::Vector2f v) {
    return (float)sqrt(v.x * v.x + v.y * v.y);
}

float Dot(sf::Vector2f v0, sf::Vector2f v1)  {
    return v0.x * v1.x + v0.x * v1.y;
}

sf::Vector2f CompMult (sf::Vector2f v1, sf::Vector2f v2) {
	return sf::Vector2f(v1.x * v2.x, v1.y * v2.y);
}

sf::Vector2f GetNormal(sf::Vector2f v) {
    return sf::Vector2f(-v.y, v.x);
}

sf::Vector2f Normalized(sf::Vector2f v) {
    return v / GetMagnitude(v);
}

class sfLine : public sf::Drawable {
public:
    sfLine(const sf::Vector2f &point1, const sf::Vector2f &point2, float thickness, sf::Color color) :
            color(color), thickness(thickness) {
        sf::Vector2f direction = point2 - point1;
        sf::Vector2f unitDirection = direction / std::sqrt(direction.x * direction.x + direction.y * direction.y);
        sf::Vector2f unitPerpendicular(-unitDirection.y, unitDirection.x);

        sf::Vector2f offset = (thickness / 2.f) * unitPerpendicular;

        vertices[0].position = point1 + offset;
        vertices[1].position = point2 + offset;
        vertices[2].position = point2 - offset;
        vertices[3].position = point1 - offset;

        for (int i = 0; i < 4; ++i)
            vertices[i].color = color;
    }

    void draw(sf::RenderTarget &target, sf::RenderStates states) const {
        target.draw(vertices, 4, sf::Quads);
    }
private:
    sf::Vertex vertices[4];
    float thickness;
    sf::Color color;
};


class ShiftedCoordinate {
public:
	ShiftedCoordinate(sf::Vector2f origin) : origin(origin) {
		f.loadFromFile("../assets/code_bold.otf");
		x1x2_text.setFont(f);
		x1x2_text.setString("");
		x1x2_text.setCharacterSize(8);
		x1x2_text.setColor(sf::Color::Black);

	};

	void flip_bit() {
		flipped = !flipped;
	}

	sf::Vector2f shift_point(sf::Vector2f point) {
		if (flipped) {
			point.y = 1 - point.y;
			return ( point * 50.0f + origin + shift * 50.0f);
		}
		else
			return point * 50.0f + origin + shift * 50.0f;
	}

	void shift_coordinate(sf::Vector2f shift) {
		this->shift = shift;
	}

	void collapse_to

	void draw(sf::RenderWindow *window) {
		
		sf::VertexArray vertex_array;
		vertex_array.setPrimitiveType(sf::LinesStrip);

		sf::Vertex v1(origin+shift*50.0f, sf::Color::Black);
		sf::Vertex vTop(origin+shift*50.0f + sf::Vector2f(0, 80), sf::Color::Black);
		sf::Vertex vRight(origin+shift*50.0f + sf::Vector2f(80, 0), sf::Color::Black);

		// Flip the Y axis
		v1.position.y = window->getSize().y - v1.position.y;
		vTop.position.y = window->getSize().y - vTop.position.y;
		vRight.position.y = window->getSize().y - vRight.position.y;

		vertex_array.append(vTop);
		vertex_array.append(v1);
		vertex_array.append(vRight);

		window->draw(vertex_array);

	}

	void draw_text(sf::RenderWindow *window, int index) {
		
		x1x2_text.setFont(f);

		x1x2_text.setPosition(origin + -shift*50.0f + sf::Vector2f(0, -80));
		x1x2_text.setString("x" + std::to_string(index+1));
		window->draw(x1x2_text);

		x1x2_text.setPosition(origin + -shift*50.0f + sf::Vector2f(80, 0));
		x1x2_text.setString("x" + std::to_string(index));
		window->draw(x1x2_text);
	}



private:
	sf::Vector2f origin;
	sf::Vector2f shift;
	bool flipped = false;

	sf::Font f;
	sf::Text x1x2_text;

	

};


class AxisLine {

public:
    AxisLine(sf::Vector2f head, sf::Vector2f tail):
            head(head), tail(tail) {

    }

    sf::Vector2f head;
    sf::Vector2f tail;

    void draw_line(sf::RenderWindow *window){

        sfLine l(head, tail, 2.0f ,sf::Color::Blue);
        window->draw(l);
    }

    sf::Vector2f get_point(float normalized_point){
        return (head - tail) / normalized_point;
    }

private:

};

class DataLine {
	
public:

	DataLine(std::vector<float> normalized_data, int data_class);

	// The coords put into (x1,x2)(x3,x4) pairs
	std::vector<sf::Vector2f> doubled_coords();

	// Upper and lower bounds to the line
	sf::Vector2f bounds();

	void shift_coords_to_match(std::vector<ShiftedCoordinate> *coords);

	void draw(std::vector<AxisLine> lines, sf::RenderWindow *window);
	void draw(std::vector<ShiftedCoordinate> coordinates, sf::RenderWindow *window);

private:

	std::vector<float> data;
	int data_class = 0;

	sf::Color color;

protected:


};

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
	
	float largest  = FLT_MAX;
	float smallest = FLT_MIN;
	
	for (auto i: data) {
		
		if (i > largest)
			largest = i;

		if (i < smallest)
			largest = i;
	}

	return sf::Vector2f(largest, smallest);
}


void DataLine::shift_coords_to_match(std::vector<ShiftedCoordinate> *coords) {

	std::vector<sf::Vector2f> paired_coordinates = doubled_coords();

	if (coords->size() < paired_coordinates.size()) {
		std::cout << "not enough axes for the amount of points";
		return;
	}

	for (int i = 1; i < paired_coordinates.size(); i++) {
		
		sf::Vector2f shift = Scale(paired_coordinates.at(0)) - Scale(paired_coordinates.at(i));
		shift.x = 0;

		coords->at(i).shift_coordinate(shift);

	}

}

void DataLine::draw(std::vector<AxisLine> lines, sf::RenderWindow *window) {





}

void DataLine::draw(std::vector<ShiftedCoordinate> coordinates, sf::RenderWindow *window) {

	std::vector<sf::Vector2f> paired_coordinates = doubled_coords();

	if (coordinates.size() < paired_coordinates.size()) {
		std::cout << "not enough axes for the amount of points";
		return;
	}

	sf::VertexArray vertex_array;
	vertex_array.setPrimitiveType(sf::LinesStrip);

	for (int i = 0; i < paired_coordinates.size(); i++) {

		// Shift the points by their respective coordinate
		sf::Vector2f point = coordinates.at(i).shift_point(Scale(paired_coordinates.at(i)));

		// Flip the Y axis
		point.y = window->getSize().y - point.y;
		
		sf::Vertex vertex(point);
		vertex.color = color;

		vertex_array.append(vertex);
		
	}

	window->draw(vertex_array);
}
