#pragma once
#include <SFML/Graphics.hpp>

class BezierAxisLine {

public:
    BezierAxisLine(sf::Vector2f head, sf::Vector2f tail, int index);

    sf::Vector2f head;
    sf::Vector2f tail;

    void draw(sf::RenderWindow *window);

    sf::Vector2f get_point(float normalized_point);
    sf::Vector2f get_midpoint(float normalized_point, BezierAxisLine next_line, float next_normalized_point);

    int get_index();
private:

    int index;
    sf::Font x_font;
    sf::Text x_text;

};
