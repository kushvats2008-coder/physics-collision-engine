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
                if (a.isstatic && b.isstatic)
                    continue;
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
                    }
                    else
                    {
                        normalX = -1.0f;
                    }
                }
                else
                {
                    correction = overlapY / 2.0f;
                    if (a.y < b.y)
                    {
                        normalY = 1.0f;
                    }
                    else
                    {
                        normalY = -1.0f;
                    }
                }

                const float correctionA = a.isstatic ? 0.0f : (b.isstatic ? correction * 2.0f : correction);
                const float correctionB = b.isstatic ? 0.0f : (a.isstatic ? correction * 2.0f : correction);
                if (overlapX < overlapY)
                {
                    if (a.x < b.x)
                    {
                        a.x -= correctionA;
                        b.x += correctionB;
                    }
                    else
                    {
                        a.x += correctionA;
                        b.x -= correctionB;
                    }
                }
                else
                {
                    if (a.y < b.y)
                    {
                        a.y -= correctionA;
                        b.y += correctionB;
                    }
                    else
                    {
                        a.y += correctionA;
                        b.y -= correctionB;
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
                    const float inverseMassA = a.isstatic ? 0.0f : 1.0f / a.mass;
                    const float inverseMassB = b.isstatic ? 0.0f : 1.0f / b.mass;
                    impulse /= inverseMassA + inverseMassB;

                    if (!a.isstatic)
                    {
                        a.velocityX -= impulse * inverseMassA * normalX;
                        a.velocityY -= impulse * inverseMassA * normalY;
                    }
                    if (!b.isstatic)
                    {
                        b.velocityX += impulse * inverseMassB * normalX;
                        b.velocityY += impulse * inverseMassB * normalY;
                    }
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
                if (distance == 0.0f)
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

                if (rect.isstatic)
                {
                    circle.x += normalX * overlap;
                    circle.y += normalY * overlap;
                }
                else
                {
                    circle.x += normalX * overlap * (rect.mass / totalMass);
                    circle.y += normalY * overlap * (rect.mass / totalMass);
                    rect.x -= normalX * overlap * (circle.mass / totalMass);
                    rect.y -= normalY * overlap * (circle.mass / totalMass);
                }

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
                    const float inverseMassCircle = 1.0f / circle.mass;
                    const float inverseMassRect = rect.isstatic ? 0.0f : 1.0f / rect.mass;
                    impulse /= inverseMassCircle + inverseMassRect;

                    circle.velocityX += impulse * inverseMassCircle * normalX;
                    circle.velocityY += impulse * inverseMassCircle * normalY;

                    if (!rect.isstatic)
                    {
                        rect.velocityX -= impulse * inverseMassRect * normalX;
                        rect.velocityY -= impulse * inverseMassRect * normalY;

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

    float wallThickness = 10.0f;

    // --- Helper to add a wall to BOTH vectors with matching physics, visuals, and center origin ---
    auto addWall = [&](float x, float y, float w, float h)
    {
        // 1. Physics wall (isStatic = true)
        rectangles.emplace_back(x, y, 0.0f, 0.0f, w, h, 0.0f, 0.0f, true);

        // 2. Visual shape matching exact width and height
        rectShapes.emplace_back(sf::Vector2f(w, h));
        rectShapes.back().setOrigin({w / 2.0f, h / 2.0f}); // Center origin
        rectShapes.back().setPosition({x, y});
        rectShapes.back().setFillColor(sf::Color::Red); // All walls red
    };

    // 1. Top wall: center at (400, 5), width 800, height 10
    addWall(400.0f, wallThickness / 2.0f, 800.0f, wallThickness);

    // 2. Bottom wall: center at (400, 595), width 800, height 10
    addWall(400.0f, 600.0f - wallThickness / 2.0f, 800.0f, wallThickness);

    // 3. Left wall: center at (5, 300), width 10, height 600
    addWall(wallThickness / 2.0f, 300.0f, wallThickness, 600.0f);

    // 4. Right wall: center at (795, 300), width 10, height 600
    addWall(800.0f - wallThickness / 2.0f, 300.0f, wallThickness, 600.0f);
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

                    addWall(400.0f, wallThickness / 2.0f, 800.0f, wallThickness);
                    addWall(400.0f, 600.0f - wallThickness / 2.0f, 800.0f, wallThickness);
                    addWall(wallThickness / 2.0f, 300.0f, wallThickness, 600.0f);
                    addWall(800.0f - wallThickness / 2.0f, 300.0f, wallThickness, 600.0f);
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

        // Display
        window.display();
    }

    return 0;
}