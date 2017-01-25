#include <SFML/Graphics.hpp>

float GetMagnitude(sf::Vector2f v) {
    return (float)sqrt(v.x * v.x + v.y * v.y);
}

float Dot(sf::Vector2f v0, sf::Vector2f v1)  {
    return v0.x * v1.x + v0.x * v1.y;
}

sf::Vector2f GetNormal(sf::Vector2f v) {
    return sf::Vector2f(-v.y, v.x);
}

sf::Vector2f Normalized(sf::Vector2f v) {
    return v / GetMagnitude(v);
}

class sfLine : public sf::Drawable {
public:
    sfLine(const sf::Vector2f &point1, const sf::Vector2f &point2) :
            color(sf::Color::Yellow), thickness(5.f) {
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

struct data_line {

    data_line(int index, int step, sf::Vector2f head, sf::Vector2f tail):
            index(index), step(step), head(head), tail(tail) {

    }

    int index = 0;
    int step = 0;
    sf::Vector2f head;
    sf::Vector2f tail;

    void draw_line(sf::RenderWindow *window){

        sfLine l(head, tail);
        window->draw(l);
    }

    sf::Vector2f get_point(float normalized_point){
        return (head - tail) / normalized_point;
    }

private:

};