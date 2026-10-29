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

    if (x <= 40)
    {
        x = 40;
        velocityX = -velocityX * resitutionX;
    }
    else if (x >= 760)
    {
        x = 760;
        velocityX = -velocityX * resitutionX;
    }
    if (y <= 40)
    {
        y = 40;
        velocityY = -velocityY * resitutionY;
    }
    else if (y >= 560)
    {
        y = 560;

        // Vertical bounce
        if (velocityY > 0)
        {
            if (velocityY < 30)
            {
                velocityY = 0;
            }
            else
            {
                velocityY = -velocityY * 0.8f;
            }
        }

        // Horizontal friction
        if (velocityX > 0)
        {
            velocityX -= friction * deltaTime;

            if (velocityX < 0)
                velocityX = 0;
        }
        else if (velocityX < 0)
        {
            velocityX += friction * deltaTime;

            if (velocityX > 0)
                velocityX = 0;
        }
    }
}

PhysicsObjectRect::PhysicsObjectRect(
    float posx, float posy,
    float velY, float velX,
    float w, float h, float Q, float W)
{
    x = posx;
    y = posy;

    velocityY = velY;
    velocityX = velX;

    width = w;
    height = h;
    angularVelocity = W;
    angle = Q;
     mass = 10.0f;
     inertia = (1.0f / 12.0f) * mass * (width * width + height * height);
}

void PhysicsObjectRect::update(float deltaTime) // rectangle
{
    float g = 490.0f;

    velocityY += g * deltaTime;
    angle = angle + (angularVelocity * deltaTime);
    y += velocityY * deltaTime;
    x += velocityX * deltaTime;
    if (x - width / 2 <= 10)
    {
        x = 10 + width / 2;
        velocityX = -velocityX * 0.2f;
    }
    else if (x + width / 2 >= 790)
    {
        x = 790 - width / 2;
        velocityX = -velocityX * 0.2f;
    }
    if (y - height / 2 <= 10)
    {
        y = 10 + height / 2;
        velocityY = -velocityY * 0.8f;
    }
    else if (y + height / 2 >= 590)
    {
        y = 590 - height / 2;

        // Vertical bounce
        if (velocityY > 0)
        {
            if (velocityY < 30)
            {
                velocityY = 0;
            }
            else
            {
                velocityY = -velocityY * 0.8f;
            }
        }

        // Horizontal friction
        if (velocityX > 0)
        {
            velocityX -= 10 * deltaTime;

            if (velocityX < 0)
                velocityX = 0;
        }
        else if (velocityX < 0)
        {
            velocityX += 10 * deltaTime;

            if (velocityX > 0)
                velocityX = 0;
        }
    }
}