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
        // LEFT CLICK in Rectangle Mode -> Spawn Rectangle
   else if (mousePressed->button == sf::Mouse::Button::Left &&
         spawnMode == SpawnMode::staticrect &&
         rectangles.size() < 20)
{
    float newWidth = 120.0f;
    float newHeight = 20.0f;

    // --- 1. OVERLAP CHECK: Can we spawn here? ---
    bool canSpawn = true;

    for (const auto &rect : rectangles)
    {
        // Only check overlap against other STATIC rectangles
        if (rect.isstatic)
        {
            // Calculate distance between centers
            float dx = std::abs(mouseX - rect.x);
            float dy = std::abs(mouseY - rect.y);

            // If the new box would overlap with an existing static box
            if (dx < (newWidth + rect.width) / 2.0f &&
                dy < (newHeight + rect.height) / 2.0f)
            {
                canSpawn = false; // Overlap detected! Block spawn!
                break;
            }
        }
    }

    // --- 2. ONLY SPAWN IF FREE OF OVERLAPS ---
    if (canSpawn)
    {
        rectangles.emplace_back(mouseX, mouseY, 0, 0, newWidth, newHeight, 0, 0, true);

        rectShapes.emplace_back(sf::Vector2f(newWidth, newHeight));
        rectShapes.back().setOrigin({newWidth / 2.0f, newHeight / 2.0f});
        rectShapes.back().setPosition({mouseX, mouseY});
        rectShapes.back().setFillColor(sf::Color(100, 100, 100)); // Dark Gray wall
    }
}
        else if (mousePressed->button == sf::Mouse::Button::Left &&
                 spawnMode == SpawnMode::staticrect &&
                 rectangles.size() < 20)
        {
            

            rectangles.emplace_back(mouseX, mouseY, 0, 0, 120, 20, 0, 0,true);
            rectShapes.emplace_back(sf::Vector2f(120.f, 20.f));
            rectShapes.back().setOrigin({60.f, 10.f});
            rectShapes.back().setPosition({mouseX, mouseY});
            rectShapes.back().setFillColor(sf::Color(
                rand() % 256, rand() % 256, rand() % 256));
        }
    }
}