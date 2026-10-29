#include <SFML/Graphics.hpp>
#include "PhysicsObject.h"
#include "InputHandler.h"
#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Collision
{
public:
    static void resolve(
        vector<PhysicsObject> &objects,
        vector<PhysicsObjectRect> &rectangles)
    {
        for (size_t i = 0; i < rectangles.size(); ++i)
        {
            for (size_t j = i + 1; j < rectangles.size(); ++j)
            {
                PhysicsObjectRect &a = rectangles[i];
                PhysicsObjectRect &b = rectangles[j];

                const float overlapX =
                    (a.width + b.width) / 2.0f - abs(a.x - b.x);
                const float overlapY =
                    (a.height + b.height) / 2.0f - abs(a.y - b.y);

                if (overlapX <= 0.0f || overlapY <= 0.0f)
                    continue;

                float normalX = 0.0f;
                float normalY = 0.0f;
                float correction = 0.0f;

                if (overlapX < overlapY)
                {
                    correction = overlapX / 2.0f;
                    if (a.x < b.x)
                    {
                        normalX = 1.0f;
                        a.x -= correction;
                        b.x += correction;
                    }
                    else
                    {
                        normalX = -1.0f;
                        a.x += correction;
                        b.x -= correction;
                    }
                }
                else
                {
                    correction = overlapY / 2.0f;
                    if (a.y < b.y)
                    {
                        normalY = 1.0f;
                        a.y -= correction;
                        b.y += correction;
                    }
                    else
                    {
                        normalY = -1.0f;
                        a.y += correction;
                        b.y -= correction;
                    }
                }

                const float relativeVelocityX = b.velocityX - a.velocityX;
                const float relativeVelocityY = b.velocityY - a.velocityY;
                const float velocityAlongNormal =
                    relativeVelocityX * normalX +
                    relativeVelocityY * normalY;

                if (velocityAlongNormal < 0.0f)
                {
                    const float restitution = 0.5f;
                    float impulse =
                        -(1.0f + restitution) * velocityAlongNormal;
                    impulse /= (1.0f / a.mass) + (1.0f / b.mass);

                    a.velocityX -= (impulse / a.mass) * normalX;
                    a.velocityY -= (impulse / a.mass) * normalY;
                    b.velocityX += (impulse / b.mass) * normalX;
                    b.velocityY += (impulse / b.mass) * normalY;
                }
            }
        }
           for (int i = 0; i < objects.size(); i++)
        {

            for (int j = i + 1; j < objects.size(); j++)
            {
                float dx = objects[j].x - objects[i].x;
                float dy = objects[j].y - objects[i].y;

                float distance = sqrt(dx * dx + dy * dy);
              if(distance == 0.0f)
                {
                    continue; // Avoid division by zero
                }
                if (distance <= objects[i].radius + objects[j].radius)
                {

                    float nx = dx / distance;
                    float ny = dy / distance;
                    float overlap =
                        objects[i].radius + objects[j].radius - distance;

                    float correctionX = nx * overlap / 2.0f;
                    float correctionY = ny * overlap / 2.0f;

                    objects[i].x -= correctionX;
                    objects[i].y -= correctionY;

                    objects[j].x += correctionX;
                    objects[j].y += correctionY;
                    // Relative velocity
                    float rvx = objects[j].velocityX - objects[i].velocityX;
                    float rvy = objects[j].velocityY - objects[i].velocityY;

                    // Relative velocity along collision direction
                    float velocityAlongNormal = rvx * nx + rvy * ny;

                    // Only respond if balls are moving toward each other
                    if (velocityAlongNormal < 0)
                    {
                        float restitution = 0.8f;

                        float impulse = -(1.0f + restitution) * velocityAlongNormal;

                        impulse /= (1.0f / objects[i].mass) +
                                   (1.0f / objects[j].mass);

                        // Apply collision impulse to ball 1
                        objects[i].velocityX -=
                            impulse * nx / objects[i].mass;

                        objects[i].velocityY -=
                            impulse * ny / objects[i].mass;

                        // Apply collision impulse to ball 2
                        objects[j].velocityX +=
                            impulse * nx / objects[j].mass;

                        objects[j].velocityY +=
                            impulse * ny / objects[j].mass;
                    }
                }
            }
        }

        for (PhysicsObject &circle : objects)
        {
            for (PhysicsObjectRect &rect : rectangles)
            {
                const float halfWidth = rect.width / 2.0f;
                const float halfHeight = rect.height / 2.0f;
                const float closestX =
                    clamp(circle.x, rect.x - halfWidth, rect.x + halfWidth);
                const float closestY =
                    clamp(circle.y, rect.y - halfHeight, rect.y + halfHeight);
                const float dx = circle.x - closestX;
                const float dy = circle.y - closestY;
                const float distanceSquared = dx * dx + dy * dy;

                if (distanceSquared >= circle.radius * circle.radius)
                    continue;

                const float distance = sqrt(distanceSquared);
                const float normalX =
                    distance != 0.0f ? dx / distance : 0.0f;
                const float normalY =
                    distance != 0.0f ? dy / distance : -1.0f;
                const float overlap = circle.radius - distance;
                const float totalMass = circle.mass + rect.mass;

                circle.x += normalX * overlap * (rect.mass / totalMass);
                circle.y += normalY * overlap * (rect.mass / totalMass);
                rect.x -= normalX * overlap * (circle.mass / totalMass);
                rect.y -= normalY * overlap * (circle.mass / totalMass);

                const float relativeVelocityX =
                    circle.velocityX - rect.velocityX;
                const float relativeVelocityY =
                    circle.velocityY - rect.velocityY;
                const float velocityAlongNormal =
                    relativeVelocityX * normalX +
                    relativeVelocityY * normalY;

                if (velocityAlongNormal < 0.0f)
                {
                    const float restitution = 0.6f;
                    float impulse =
                        -(1.0f + restitution) * velocityAlongNormal;
                    impulse /= (1.0f / circle.mass) + (1.0f / rect.mass);

                    circle.velocityX += (impulse / circle.mass) * normalX;
                    circle.velocityY += (impulse / circle.mass) * normalY;
                    rect.velocityX -= (impulse / rect.mass) * normalX;
                    rect.velocityY -= (impulse / rect.mass) * normalY;

                    const float offsetX = closestX - rect.x;
                    const float offsetY = closestY - rect.y;
                    const float torque =
                        (offsetX * (-impulse * normalY) -
                         offsetY * (-impulse * normalX)) *
                        (180.0f / 3.14159265f);
                    rect.angularVelocity += torque / rect.inertia;
                }
            }
        }
    }
};

int main()
{
    vector<PhysicsObject> objects;
    vector<sf::CircleShape> balls;
    vector<PhysicsObjectRect> rectangles;
    vector<sf::RectangleShape> rectShapes;
    SpawnMode spawnMode = SpawnMode::Circle;

    // Create SFML window
    sf::RenderWindow window(
        sf::VideoMode({800, 600}),
        "My Physics Engine");

    // Create walls
    float wallThickness = 10.f;

    sf::RectangleShape topWall(
        sf::Vector2f(800.f, wallThickness));

    sf::RectangleShape bottomWall(
        sf::Vector2f(800.f, wallThickness));

    sf::RectangleShape leftWall(
        sf::Vector2f(wallThickness, 600.f));

    sf::RectangleShape rightWall(
        sf::Vector2f(wallThickness, 600.f));

    // Position walls
    topWall.setPosition({0.f, 0.f});
    bottomWall.setPosition({0.f, 590.f});
    leftWall.setPosition({0.f, 0.f});
    rightWall.setPosition({790.f, 0.f});

    // Wall colors
    topWall.setFillColor(sf::Color::Red);
    bottomWall.setFillColor(sf::Color::Red);
    leftWall.setFillColor(sf::Color::Red);
    rightWall.setFillColor(sf::Color::Red);

    // Clock
    sf::Clock clock;

    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();

        // Handle events
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            if (const auto *keyPressed =
                    event->getIf<sf::Event::KeyPressed>())
            {

                if (keyPressed->code == sf::Keyboard::Key::R)
                {
                    objects.clear();
                    balls.clear();
                    rectangles.clear();
                    rectShapes.clear();
                }
            }

            InputHandler::handleSpawnInput(
                *event,
                spawnMode,
                objects,
                balls,
                rectangles,
                rectShapes);
        }

        // Controls for first ball
       // Controls for player rectangle (Arrows to move, Q/E to spin)
if (!rectangles.empty())
{
    InputHandler::handleInput(rectangles[0], deltaTime);
}

        // Update physics
        for (auto &object : objects)
        {
            object.update(deltaTime);
        }

        for (auto &rectangle : rectangles)
        {
            rectangle.update(deltaTime);
        }

        Collision::resolve(objects, rectangles);

        // Update visual positions
        for (int i = 0; i < balls.size(); i++)
        {
            balls[i].setPosition({objects[i].x,
                                  objects[i].y});
        }

        for (int i = 0; i < rectangles.size(); i++)
        {
            rectShapes[i].setPosition({rectangles[i].x, rectangles[i].y});
            rectShapes[i].setRotation(sf::degrees(rectangles[i].angle));
        }

        // Clear
        window.clear();

        // Draw balls
        for (int i = 0; i < balls.size(); i++)
        {
            window.draw(balls[i]);
        }

        for (auto &rectShape : rectShapes)
        {
            window.draw(rectShape);
        }

        // Draw walls
        window.draw(topWall);
        window.draw(bottomWall);
        window.draw(leftWall);
        window.draw(rightWall);

        // Display
        window.display();
    }

    return 0;
}