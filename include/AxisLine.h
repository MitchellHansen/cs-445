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

    sf::Vector2f shift {0, 0};

	void draw_line(sf::RenderWindow *window);

	sf::Vector2f get_point(float normalized_point);

    void shift_line(sf::Vector2f shift);

private:

};

