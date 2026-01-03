#include "quadtree.hpp"
#include "boid.hpp"

QuadTree::QuadTree(sf::Vector2f topL, sf::Vector2f bottomR, int cap)
    : capacity(cap), topLeft(topL), bottomRight(bottomR) {}

void QuadTree::subdivide()
{
    sf::Vector2f mid((topLeft.x + bottomRight.x) / 2.f, (topLeft.y + bottomRight.y) / 2.f);

    northwest = std::make_unique<QuadTree>(topLeft, mid, capacity);
    northeast = std::make_unique<QuadTree>(sf::Vector2f(mid.x, topLeft.y), sf::Vector2f(bottomRight.x, mid.y), capacity);
    southwest = std::make_unique<QuadTree>(sf::Vector2f(topLeft.x, mid.y), sf::Vector2f(mid.x, bottomRight.y), capacity);
    southeast = std::make_unique<QuadTree>(mid, bottomRight, capacity);

    isSubdivided = true;

    for (Boid *b : flock)
    {
        insert(b);
    }
    flock.clear();
}

void QuadTree::insert(Boid *b)
{
    if (!inBounds(b->getPosition()))
        return;

    if (!isSubdivided)
    {
        if (flock.size() < (size_t)capacity)
        {
            flock.push_back(b);
            return;
        }
        subdivide();
    }

    northwest->insert(b);
    northeast->insert(b);
    southwest->insert(b);
    southeast->insert(b);
}

void QuadTree::query(const Boid &b, std::vector<Boid *> &found) const
{
    if (!intersects(b))
        return;

    for (Boid *other : flock)
    {
        if (other->getId() != b.getId())
        {
            sf::Vector2f diff = b.getPosition() - other->getPosition();
            float distSq = (diff.x * diff.x) + (diff.y * diff.y);
            if (distSq < (float)(b.getRadius() * b.getRadius()))
            {
                found.push_back(other);
            }
        }
    }

    if (isSubdivided)
    {
        northwest->query(b, found);
        northeast->query(b, found);
        southwest->query(b, found);
        southeast->query(b, found);
    }
}

bool QuadTree::inBounds(sf::Vector2f coords) const
{
    return (coords.x >= topLeft.x && coords.x <= bottomRight.x && coords.y >= topLeft.y && coords.y <= bottomRight.y);
}

bool QuadTree::intersects(const Boid &b) const
{
    float r = (float)b.getRadius();
    sf::Vector2f pos = b.getPosition();
    return !(pos.x + r < topLeft.x || pos.x - r > bottomRight.x || pos.y + r < topLeft.y || pos.y - r > bottomRight.y);
}