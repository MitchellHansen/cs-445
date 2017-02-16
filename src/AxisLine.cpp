#include <AxisLine.h>

void AxisLine::draw_line(sf::RenderWindow *window) {
    sfLine l1(head+shift, tail+shift, 2.0f, sf::Color::Blue);
    window->draw(l1);
}

sf::Vector2f AxisLine::get_point(float normalized_point)  {
    sf::Vector2f val = ((tail - head) * normalized_point) + head;
    return val + shift;
}

void AxisLine::shift_line(sf::Vector2f shift) {
    this->shift = shift;
}

sf::Vector2f AxisLine::get_midpoint(float normalized_point, AxisLine next_line, float next_normalized_point) {

    sf::Vector2f point1 = get_point(normalized_point);

    sf::Vector2f point2 = next_line.get_point(next_normalized_point);

    sf::Vector2f difference = point2 - point1;

    // get the difference, use the x as the midpoints y
    sf::Vector2f midpoint(fabs(difference.y) + point1.x, point1.y + (point2.y - point1.y)/2);

    return midpoint;
}
