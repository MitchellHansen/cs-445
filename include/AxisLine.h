#pragma once
#include <SFML/Graphics.hpp>
#include "DataLine.h"
#include "util.hpp"

class AxisLine {

public:
	AxisLine(sf::Vector2f head, sf::Vector2f tail) :
		head(head), tail(tail) {

	}

	sf::Vector2f head;
	sf::Vector2f tail;

	void draw_line(sf::RenderWindow *window) {

		sfLine l(head, tail, 2.0f, sf::Color::Blue);
		window->draw(l);
	}

	sf::Vector2f get_point(float normalized_point) {
		return (head - tail) / normalized_point;
	}

private:

};

