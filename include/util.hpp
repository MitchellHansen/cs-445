#pragma once
#include <SFML/Graphics.hpp>

const int WINDOW_SIZE_X = 800;
const int WINDOW_SIZE_Y = 800;
const float SCALE_VALUE = 50.0f;


inline sf::Vector2f Scale(sf::Vector2f in) {
	return sf::Vector2f(in.x, in.y * 1.0f);
}
inline float GetMagnitude(sf::Vector2f v) {
    return (float)sqrt(v.x * v.x + v.y * v.y);
}

inline float Dot(sf::Vector2f v0, sf::Vector2f v1)  {
    return v0.x * v1.x + v0.x * v1.y;
}

inline sf::Vector2f CompMult (sf::Vector2f v1, sf::Vector2f v2) {
	return sf::Vector2f(v1.x * v2.x, v1.y * v2.y);
}

inline sf::Vector2f GetNormal(sf::Vector2f v) {
    return sf::Vector2f(-v.y, v.x);
}

inline sf::Vector2f Normalized(sf::Vector2f v) {
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


