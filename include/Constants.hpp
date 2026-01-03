#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

namespace Config
{
    // Window Settings
    inline constexpr float WORLD_WIDTH = 1920.0f;
    inline constexpr float WORLD_HEIGHT = 1080.0f;
    inline constexpr int FPS_LIMIT = 60;

    // Simulation Settings
    inline constexpr int BOID_COUNT = 5000;
    inline constexpr int QUADTREE_CAPACITY = 64;

    // Boid Physics
    inline constexpr float MAX_SPEED = 4.0f;
    inline constexpr float VISUAL_RANGE = 20.0f;

    // Behavior Weights
    inline constexpr float SEPARATION_WEIGHT = 0.5f;
    inline constexpr float ALIGNMENT_WEIGHT = 0.05f;
    inline constexpr float COHESION_WEIGHT = 0.01f;
}

#endif