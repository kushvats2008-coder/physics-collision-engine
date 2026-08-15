#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(float posx, float posy, float velY,     float velX, float accX,float accY,float f , float r)
{
    float g = 490.0f;
    x = posx;
    y = posy;
    velocityY = velY;
    velocityX = velX;
    accelerationX ;
    accelerationY ;
    friction = f;
    mass = 20.0f;
    forceY = mass * g;
    forceX = 100.0f;
    accelerationY = 0;
    accelerationX = 0;
    resitutionX = 0.2f;
    resitutionY = 0.8f;
    radius = r ;
}

void PhysicsObject::update(float deltaTime) //core logic 
{
  accelerationY = forceY / mass;
  accelerationX = forceX / mass;


   velocityY += accelerationY * deltaTime;
   velocityX += accelerationX * deltaTime;
    y += velocityY * deltaTime;
    x += velocityX * deltaTime;
   

    if(x<=10 )
    {
        x=10;
        velocityX = -velocityX * resitutionX;
        
        
    }
   else if (x >= 730)
{
    x = 730;
    velocityX = -velocityX * resitutionX;

   
}
    if (y<=10)
    {
        y=10;
        velocityY = -velocityY * resitutionY;
    }
    else if (y >= 530)
{
    y = 530;

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