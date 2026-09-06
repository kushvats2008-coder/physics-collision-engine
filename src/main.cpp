#include <SFML/Graphics.hpp>
#include "PhysicsObject.h"
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <vector>
using namespace std;
int main()
{
    vector<PhysicsObject> objects;
    vector<sf::CircleShape> balls;

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
                }
            }

            if (const auto *mousePressed =
                    event->getIf<sf::Event::MouseButtonPressed>())
            {
                float mouseX =
                    static_cast<float>(mousePressed->position.x); // creat visual balls.

                float mouseY =
                    static_cast<float>(mousePressed->position.y);
                if (mousePressed->button == sf::Mouse::Button::Left)
                {
                    objects.emplace_back(
                        mouseX, mouseY,
                        0, 100,
                        0, 0,
                        100, 30);

                    balls.emplace_back(30.f);

                    balls.back().setOrigin({30.f, 30.f});
                    balls.back().setPosition({mouseX, mouseY});
                    balls.back().setFillColor(sf::Color(
                        rand() % 256,
                        rand() % 256,
                        rand() % 256));
                }
                else if (mousePressed->button == sf::Mouse::Button::Right)
                {
                    for (int i = 0; i < objects.size(); i++)
                    {
                        float dx = objects[i].x - mouseX;
                        float dy = objects[i].y - mouseY;

                        float distance = sqrt(dx * dx + dy * dy);

                        if (distance <= objects[i].radius)
                        {
                            objects.erase(objects.begin() + i);
                            balls.erase(balls.begin() + i);
                            break;
                        }
                    }
                }
            }
        }

        // Controls for first ball
        if (!objects.empty())
        {
            if (sf::Keyboard::isKeyPressed(
                    sf::Keyboard::Key::Right))
            {
                objects[0].velocityX += 10;
            }
            else if (sf::Keyboard::isKeyPressed(
                         sf::Keyboard::Key::Left))
            {
                objects[0].velocityX -= 10;
            }

            if (sf::Keyboard::isKeyPressed(
                    sf::Keyboard::Key::Up))
            {
                objects[0].velocityY -= 10;
            }
            else if (sf::Keyboard::isKeyPressed(
                         sf::Keyboard::Key::Down))
            {
                objects[0].velocityY += 10;
            }
        }

        // Update physics
        for (auto &object : objects)
        {
            object.update(deltaTime);
        }
        for (int i = 0; i < objects.size(); i++)
        {

            for (int j = i + 1; j < objects.size(); j++)
            {
                float dx = objects[j].x - objects[i].x;
                float dy = objects[j].y - objects[i].y;

                float distance = sqrt(dx * dx + dy * dy);

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

        // Update visual positions
        for (int i = 0; i < balls.size(); i++)
        {
            balls[i].setPosition({objects[i].x,
                                  objects[i].y});
        }

        // Clear
        window.clear();

        // Draw balls
        for (int i = 0; i < balls.size(); i++)
        {
            window.draw(balls[i]);
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