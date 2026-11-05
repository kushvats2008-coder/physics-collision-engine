#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(float posx, float posy, float velY, float velX, float accX, float accY, float f, float r)
{
    float g = 490.0f;
    x = posx;
    y = posy;
    velocityY = velY;
    velocityX = velX;
    accelerationX;
    accelerationY;
    friction = f;
    mass = 20.0f;
    forceY = mass * g;
    forceX = 100.0f;
    accelerationY = 0;
    accelerationX = 0;
    resitutionX = 0.2f;
    resitutionY = 0.8f;
    radius = r;
}

void PhysicsObject::update(float deltaTime) // core logic circle
{
    
    accelerationY = forceY / mass;
    accelerationX = forceX / mass;

    velocityY += accelerationY * deltaTime;
    velocityX += accelerationX * deltaTime;
    y += velocityY * deltaTime;
    x += velocityX * deltaTime;

    
}

PhysicsObjectRect::PhysicsObjectRect(
    float posx, float posy,
    float velY, float velX,
    float w, float h, float Q, float W, bool staticBody)
{
    // 1. Initialize everything for ALL rectangles (even static ones!)
    x = posx;
    y = posy;

    velocityY = velY;
    velocityX = velX;

    width = w;
    height = h;
    angularVelocity = W;
    angle = Q;

    isstatic = staticBody;

    mass = 10.0f;
    inertia = (1.0f / 12.0f) * mass * (width * width + height * height);
}

void PhysicsObjectRect::update(float deltaTime) // rectangle
{
    if (isstatic) return; // Frozen in time: no gravity, no velocity, no spin!

    float g = 490.0f;

    velocityY += g * deltaTime;
    angle = angle + (angularVelocity * deltaTime);
    y += velocityY * deltaTime;
    x += velocityX * deltaTime;
}