#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "PhysicsObject.h"
#include <SFML/Graphics.hpp>
#include <vector>

enum struct SpawnMode
{
    Circle,
    Rectangle
};

class InputHandler
{
public:
    static void handleInput(PhysicsObjectRect &player, float deltaTime);

    static void handleSpawnInput(
        const sf::Event &event,
        SpawnMode &spawnMode,
        std::vector<PhysicsObject> &objects,
        std::vector<sf::CircleShape> &balls,
        std::vector<PhysicsObjectRect> &rectangles,
        std::vector<sf::RectangleShape> &rectShapes);
};

#endif
