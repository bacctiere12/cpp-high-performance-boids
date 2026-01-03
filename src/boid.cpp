#include "boid.hpp"
#include "quadtree.hpp"
#include "Constants.hpp"
#include <cmath>
#include <random>

Boid::Boid(unsigned int id) : id(id)
{
    static std::mt19937 rng(std::random_device{}());
    static std::uniform_real_distribution<float> distWidth(0.f, Config::WORLD_WIDTH);
    static std::uniform_real_distribution<float> distHeight(0.f, Config::WORLD_HEIGHT);
    static std::uniform_real_distribution<float> distVel(-2.f, 2.f);

    position = sf::Vector2f(distWidth(rng), distHeight(rng));
    velocity = sf::Vector2f(distVel(rng), distVel(rng));

    neighbors.reserve(static_cast<std::size_t>(Config::QUADTREE_CAPACITY) * 2);
}

void Boid::applyBehaviors(const std::vector<Boid *> &neighbors)
{
    if (neighbors.empty())
        return;

    sf::Vector2f separation(0.0f, 0.0f);
    sf::Vector2f alignment(0.0f, 0.0f);
    sf::Vector2f cohesion(0.0f, 0.0f);

    const float visualRangeSq = Config::VISUAL_RANGE * Config::VISUAL_RANGE;

    for (Boid *n : neighbors)
    {
        sf::Vector2f diff = position - n->getPosition();
        float distSq = (diff.x * diff.x) + (diff.y * diff.y);

        if (distSq > 0.0f && distSq < visualRangeSq)
        {
            separation += (diff / distSq);
        }

        alignment += n->getVelocity();
        cohesion += n->getPosition();
    }

    float factor = 1.0f / static_cast<float>(neighbors.size());
    alignment = (alignment * factor - velocity) * Config::ALIGNMENT_WEIGHT;
    cohesion = (cohesion * factor - position) * Config::COHESION_WEIGHT;
    separation *= Config::SEPARATION_WEIGHT;

    velocity += (alignment + cohesion + separation);
}

void Boid::update(const QuadTree &qt, sf::VertexArray &vertices)
{
    // 1. Logic updates
    neighbors.clear();
    qt.query(*this, neighbors);
    applyBehaviors(neighbors);

    float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    if (speed > Config::MAX_SPEED)
    {
        velocity = (velocity / speed) * Config::MAX_SPEED;
    }

    position += velocity;
    stayInBounds(Config::WORLD_WIDTH, Config::WORLD_HEIGHT);

    // 2. Batch Rendering update
    std::size_t idx = static_cast<std::size_t>(id) * 3;
    float size = 6.0f;
    float halfSize = size / 2.0f;

    sf::Vector2f forward = (speed > 0.0f) ? (velocity / speed) : sf::Vector2f(0.0f, -1.0f);
    sf::Vector2f side = sf::Vector2f(-forward.y, forward.x);

    vertices[idx].position = position + (forward * size);
    vertices[idx + 1].position = position - (forward * halfSize) + (side * halfSize);
    vertices[idx + 2].position = position - (forward * halfSize) - (side * halfSize);

    // Set Colors
    vertices[idx].color = sf::Color::Green;
    vertices[idx + 1].color = sf::Color(0, 150, 0);
    vertices[idx + 2].color = sf::Color(0, 150, 0);
}

void Boid::stayInBounds(float w, float h)
{
    if (position.x < 0.0f)
        position.x = w;
    if (position.x > w)
        position.x = 0.0f;
    if (position.y < 0.0f)
        position.y = h;
    if (position.y > h)
        position.y = 0.0f;
}