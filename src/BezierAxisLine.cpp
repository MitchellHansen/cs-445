#pragma once

#include <util.hpp>
#include "BezierAxisLine.h"

BezierAxisLine::BezierAxisLine(sf::Vector2f head, sf::Vector2f tail, int index) :
        head(head), tail(tail), index(index) {

    x_font.loadFromFile("../assets/code_bold.otf");
    x_text.setFont(x_font);
    x_text.setString("");
    x_text.setCharacterSize(8);
    x_text.setColor(sf::Color::Black);

}

void BezierAxisLine::draw(sf::RenderWindow *window) {
    sfLine l1(head, tail, 2.0f, sf::Color::Black);
    sfLine l2(head + sf::Vector2f(0, 20), head - sf::Vector2f(0, 20), 1.0f, sf::Color::Black);
    sfLine l3(tail + sf::Vector2f(0, 20), tail - sf::Vector2f(0, 20), 1.0f, sf::Color::Black);
    window->draw(l1);
    window->draw(l2);
    window->draw(l3);

    x_text.setFont(x_font);

    x_text.setPosition(tail);
    x_text.setString("x" + std::to_string(index));
    window->draw(x_text);

}

sf::Vector2f BezierAxisLine::get_point(float normalized_point)  {
    sf::Vector2f val = ((tail - head) * normalized_point) + head;
    return val;
}

sf::Vector2f BezierAxisLine::get_midpoint(float normalized_point, BezierAxisLine next_line, float next_normalized_point){

    sf::Vector2f point1 = get_point(normalized_point);

    sf::Vector2f point2 = next_line.get_point(next_normalized_point);

    sf::Vector2f difference = point2 - point1;

    // get the difference, use the x as the midpoints y
    sf::Vector2f midpoint(head.x, difference.x);

    return midpoint;

}

int BezierAxisLine::get_index() {
    return index;
}
