#include <SFML/Graphics.hpp>
#include "PhysicsObject.h"
#include <cmath>
#include <iostream>
using namespace std;
int main()
{
    // Create two physics objects
    PhysicsObject ballPhysics(
        250, 300,
        0, 100,
        0, 0,
        100,30
    );

    PhysicsObject ballPhysics2(
        550, 300,
        0, -100,
        0, 0,
        100,30
    );

    // Create SFML window
    sf::RenderWindow window(
        sf::VideoMode({800, 600}),
        "My Physics Engine"
    );

    // Create visual balls
    sf::CircleShape ball(30.f);
    ball.setFillColor(sf::Color::White);

    sf::CircleShape ball2(30.f);
    ball2.setFillColor(sf::Color::Green);

    // Create walls
    float wallThickness = 10.f;

    sf::RectangleShape topWall(
        sf::Vector2f(800.f, wallThickness)
    );

    sf::RectangleShape bottomWall(
        sf::Vector2f(800.f, wallThickness)
    );

    sf::RectangleShape leftWall(
        sf::Vector2f(wallThickness, 600.f)
    );

    sf::RectangleShape rightWall(
        sf::Vector2f(wallThickness, 600.f)
    );

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

            if (const auto* keyPressed =
                event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->code == sf::Keyboard::Key::R)
                {
                    ballPhysics.velocityX = 100;
                    ballPhysics.velocityY = 0;
                    ballPhysics.x = 250;
                    ballPhysics.y = 300;

                    ballPhysics2.velocityX = -100;
                    ballPhysics2.velocityY = 0;
                    ballPhysics2.x = 550;
                    ballPhysics2.y = 300;
                }
            }
        }

        // Controls for first ball
        if (sf::Keyboard::isKeyPressed(
                sf::Keyboard::Key::Right))
        {
            ballPhysics.velocityX += 10;
        }
        else if (sf::Keyboard::isKeyPressed(
                     sf::Keyboard::Key::Left))
        {
            ballPhysics.velocityX -= 10;
        }

        if (sf::Keyboard::isKeyPressed(
                sf::Keyboard::Key::Up))
        {
            ballPhysics.velocityY -= 10;
        }
        else if (sf::Keyboard::isKeyPressed(
                     sf::Keyboard::Key::Down))
        {
            ballPhysics.velocityY += 10;
        }

        // Update physics
        ballPhysics.update(deltaTime);
        ballPhysics2.update(deltaTime);

        float dx = ballPhysics2.x - ballPhysics.x;
        float dy = ballPhysics2.y - ballPhysics.y;

        float distance = sqrt(dx * dx + dy * dy);

        if(distance <= ballPhysics.radius + ballPhysics2.radius)
        {
            
         float nx = dx / distance;
    float ny = dy / distance;
float overlap =
    ballPhysics.radius + ballPhysics2.radius - distance;

float correctionX = nx * overlap / 2.0f;
float correctionY = ny * overlap / 2.0f;

ballPhysics.x -= correctionX;
ballPhysics.y -= correctionY;

ballPhysics2.x += correctionX;
ballPhysics2.y += correctionY;
    // Relative velocity
    float rvx = ballPhysics2.velocityX - ballPhysics.velocityX;
    float rvy = ballPhysics2.velocityY - ballPhysics.velocityY;

    // Relative velocity along collision direction
    float velocityAlongNormal = rvx * nx + rvy * ny;

    // Only respond if balls are moving toward each other
    if (velocityAlongNormal < 0)
    {
        float restitution = 0.8f;

        float impulse = -(1.0f + restitution) * velocityAlongNormal;

        impulse /= (1.0f / ballPhysics.mass) +
                   (1.0f / ballPhysics2.mass);

        // Apply collision impulse to ball 1
        ballPhysics.velocityX -=
            impulse * nx / ballPhysics.mass;

        ballPhysics.velocityY -=
            impulse * ny / ballPhysics.mass;

        // Apply collision impulse to ball 2
        ballPhysics2.velocityX +=
            impulse * nx / ballPhysics2.mass;

        ballPhysics2.velocityY +=
            impulse * ny / ballPhysics2.mass;
    } 
        }

        
        // Update visual positions
        ball.setPosition({
            ballPhysics.x,
            ballPhysics.y
        });

        ball2.setPosition({
            ballPhysics2.x,
            ballPhysics2.y
        });

        // Clear
        window.clear();

        // Draw balls
        window.draw(ball);
        window.draw(ball2);

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