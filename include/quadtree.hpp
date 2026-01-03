#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>

class Boid; // Forward declaration

class QuadTree
{
private:
    int capacity;
    std::vector<Boid *> flock; // Storing pointers for performance

    sf::Vector2f topLeft;
    sf::Vector2f bottomRight;

    std::unique_ptr<QuadTree> northwest;
    std::unique_ptr<QuadTree> northeast;
    std::unique_ptr<QuadTree> southwest;
    std::unique_ptr<QuadTree> southeast;

    bool isSubdivided = false;

public:
    QuadTree(sf::Vector2f topL, sf::Vector2f bottomR, int cap);

    // Disable copying for performance and memory safety
    QuadTree(const QuadTree &) = delete;
    QuadTree &operator=(const QuadTree &) = delete;

    void insert(Boid *b);
    void query(const Boid &b, std::vector<Boid *> &found) const;
    void subdivide();

    bool inBounds(sf::Vector2f coords) const;
    bool intersects(const Boid &b) const;
};

#endif