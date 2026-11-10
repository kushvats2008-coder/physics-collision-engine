#pragma once
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "PhysicsObject.h"

class SaveManager
{
    public:
    static bool saveToFile(
              const std::string &filename,
        const std::vector<PhysicsObject> &objects,
        const std::vector<sf::CircleShape> &balls,
        const std::vector<PhysicsObjectRect> &rectangles,
        const std::vector<sf::RectangleShape> &rectShapes);

    static bool loadFromFile(
              const std::string &filename,
        std::vector<PhysicsObject> &objects,
        std::vector<sf::CircleShape> &balls,
        std::vector<PhysicsObjectRect> &rectangles,
        std::vector<sf::RectangleShape> &rectShapes);
    };