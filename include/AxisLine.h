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

	void draw_line(sf::RenderWindow *window) {

		sfLine l(head+shift, tail+shift, 2.0f, sf::Color::Blue);
		window->draw(l);
	}

	sf::Vector2f get_point(float normalized_point) {
		sf::Vector2f val = ((tail - head) * normalized_point) + head;
        return val + shift;
	}

    void shift_line(sf::Vector2f shift){
        this->shift = shift;
    }

private:

};

