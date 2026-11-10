#include "InputHandler.h"
#include <SFML/Window/Keyboard.hpp>
#include <cmath>
#include <cstdlib>

// --- Keyboard Controls for Player Rectangle (Arrows + Q/E) ---
void InputHandler::handleInput(PhysicsObjectRect &player, float deltaTime)
{
    constexpr float movementAcceleration = 600.0f;
    constexpr float rotationAcceleration = 720.0f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        player.velocityX += movementAcceleration * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        player.velocityX -= movementAcceleration * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
        player.velocityY -= movementAcceleration * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
        player.velocityY += movementAcceleration * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))
        player.angularVelocity += rotationAcceleration * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
        player.angularVelocity -= rotationAcceleration * deltaTime;
}

// --- Spawn Controls (Mouse + Z/X Mode Switch) ---
void InputHandler::handleSpawnInput(
    const sf::Event &event,
    SpawnMode &spawnMode,
    std::vector<PhysicsObject> &objects,
    std::vector<sf::CircleShape> &balls,
    std::vector<PhysicsObjectRect> &rectangles,
    std::vector<sf::RectangleShape> &rectShapes)
{
    // Mode Switching: Z = Circle, X = Rectangle
    if (const auto *keyPressed = event.getIf<sf::Event::KeyPressed>())
    {
        if (keyPressed->code == sf::Keyboard::Key::Z)
            spawnMode = SpawnMode::Circle;
        else if (keyPressed->code == sf::Keyboard::Key::X)
            spawnMode = SpawnMode::Rectangle;
            else if(keyPressed->code == sf::Keyboard::Key::C)
            spawnMode = SpawnMode::staticrect;
    }

    // Mouse Spawning
    if (const auto *mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
    {
        const float mouseX = static_cast<float>(mousePressed->position.x);
        const float mouseY = static_cast<float>(mousePressed->position.y);

        // 1. LEFT CLICK in Circle Mode -> Spawn Circle
        if (mousePressed->button == sf::Mouse::Button::Left &&
            spawnMode == SpawnMode::Circle && objects.size() < 10)
        {
            objects.emplace_back(
                mouseX, mouseY, 0, 100, 0, 0, 100,
                PhysicsObject::defaultRadius);
            balls.emplace_back(PhysicsObject::defaultRadius);
            balls.back().setOrigin(
                {PhysicsObject::defaultRadius, PhysicsObject::defaultRadius});
            balls.back().setPosition({mouseX, mouseY});
            balls.back().setFillColor(sf::Color(
                rand() % 256, rand() % 256, rand() % 256));
        }
        // LEFT CLICK in Rectangle Mode -> Spawn dynamic rectangle
        else if (mousePressed->button == sf::Mouse::Button::Left &&
                 spawnMode == SpawnMode::Rectangle &&
                 rectangles.size() < 20)
        {
            constexpr float newWidth = 60.0f;
            constexpr float newHeight = 40.0f;

            rectangles.emplace_back(
                mouseX, mouseY, 0, 0, newWidth, newHeight, 0, 0, false);
            rectShapes.emplace_back(sf::Vector2f(newWidth, newHeight));
            rectShapes.back().setOrigin({newWidth / 2.0f, newHeight / 2.0f});
            rectShapes.back().setPosition({mouseX, mouseY});
            rectShapes.back().setFillColor(sf::Color(
                rand() % 256, rand() % 256, rand() % 256));
        }
        // LEFT CLICK in Static Rectangle Mode -> Spawn static rectangle
        else if (mousePressed->button == sf::Mouse::Button::Left &&
                 spawnMode == SpawnMode::staticrect &&
                 rectangles.size() < 20)
        {
            constexpr float newWidth = 120.0f;
            constexpr float newHeight = 20.0f;

            rectangles.emplace_back(
                mouseX, mouseY, 0, 0, newWidth, newHeight, 0, 0, true);
            rectShapes.emplace_back(sf::Vector2f(newWidth, newHeight));
            rectShapes.back().setOrigin({newWidth / 2.0f, newHeight / 2.0f});
            rectShapes.back().setPosition({mouseX, mouseY});
            rectShapes.back().setFillColor(sf::Color(
                rand() % 256, rand() % 256, rand() % 256));
        }
    }
}