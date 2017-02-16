#pragma once
#include <SFML/Graphics.hpp>

const int WINDOW_SIZE_X = 800;
const int WINDOW_SIZE_Y = 800;
const float SCALE_VALUE = 50.0f;

inline std::vector<sf::Vector2f> CalcCubicBezier(
        const sf::Vector2f &start,
        const sf::Vector2f &end,
        const sf::Vector2f &startControl,
        const sf::Vector2f &endControl,
        const size_t numSegments)
{
    std::vector<sf::Vector2f> ret;
    if (!numSegments) // Any points at all?
        return ret;

    ret.push_back(start); // First point is fixed
    float p = 1.f / numSegments;
    float q = p;
    for (size_t i = 1; i < numSegments; i++, p += q) // Generate all between
        ret.push_back(p * p * p * (end + 3.f * (startControl - endControl) - start) +
                      3.f * p * p * (start - 2.f * startControl + endControl) +
                      3.f * p * (startControl - start) + start);
    ret.push_back(end); // Last point is fixed
    return ret;
}

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


