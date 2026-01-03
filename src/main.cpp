#include <SFML/Graphics.hpp>
#include <vector>
#include <execution>
#include <algorithm>
#include "boid.hpp"
#include "quadtree.hpp"
#include "Constants.hpp"

int main()
{
    unsigned int width = static_cast<unsigned int>(Config::WORLD_WIDTH);
    unsigned int height = static_cast<unsigned int>(Config::WORLD_HEIGHT);

    sf::RenderWindow window(sf::VideoMode(width, height), "Industrial Boids Simulation");

    window.setFramerateLimit(Config::FPS_LIMIT);
    window.setVerticalSyncEnabled(true);

    std::vector<Boid> boids;
    boids.reserve(Config::BOID_COUNT);
    for (int i = 0; i < Config::BOID_COUNT; ++i)
    {
        boids.emplace_back(static_cast<unsigned int>(i));
    }

    sf::VertexArray vertices(sf::Triangles, Config::BOID_COUNT * 3);

    sf::Clock frameClock;
    sf::Clock fpsClock;
    int frameCount = 0;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (frameClock.getElapsedTime() >= sf::milliseconds(16))
        {
            window.clear(sf::Color(20, 20, 20));

            // Use explicit constructor instead of braces {}
            QuadTree qt(
                sf::Vector2f(0.0f, 0.0f),
                sf::Vector2f(Config::WORLD_WIDTH, Config::WORLD_HEIGHT),
                Config::QUADTREE_CAPACITY);

            for (auto &b : boids)
            {
                qt.insert(&b);
            }

            std::for_each(std::execution::par, boids.begin(), boids.end(), [&](Boid &b)
                          { b.update(qt, vertices); });

            window.draw(vertices);

            frameCount++;
            float elapsed = fpsClock.getElapsedTime().asSeconds();
            if (elapsed >= 1.0f)
            {
                int fps = static_cast<int>(static_cast<float>(frameCount) / elapsed);
                window.setTitle("Boids | FPS: " + std::to_string(fps) + " | Agents: " + std::to_string(Config::BOID_COUNT));
                frameCount = 0;
                fpsClock.restart();
            }

            window.display();
            frameClock.restart();
        }
    }
    return 0;
}