#ifndef BOID_HPP
#define BOID_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Constants.hpp"

class QuadTree;

class Boid
{
private:
    sf::Vector2f position;
    sf::Vector2f velocity;
    unsigned int id;
    std::vector<Boid *> neighbors;

public:
    Boid(unsigned int id);

    sf::Vector2f getPosition() const { return position; }
    sf::Vector2f getVelocity() const { return velocity; }
    unsigned int getId() const { return id; }
    int getRadius() const { return Config::VISUAL_RANGE; }

    void update(const QuadTree &qt, sf::VertexArray &vertices);

    void stayInBounds(float width, float height);
    void applyBehaviors(const std::vector<Boid *> &neighbors);
};

#endif