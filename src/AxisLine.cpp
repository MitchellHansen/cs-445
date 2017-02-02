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