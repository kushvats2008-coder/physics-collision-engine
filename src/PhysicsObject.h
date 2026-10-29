#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H

class PhysicsObject
{
public:
    float x;
    float y;
    float velocityY;
    float velocityX;
    float deltaTime;
    float accelerationX;
    float accelerationY;
    float minimumbouncespeed = 3;
    float friction;
    float mass;
    float forceX;
    float forceY;
    float radius;
    float resitutionX;
    float resitutionY;
    float width;
    float height;
    PhysicsObject(float posx, float posy, float velY, float velX, float accX, float accY, float f, float r);

    void update(float deltaTime);
};
class PhysicsObjectRect
{
public:
    float x;
    float y;
    float width;
    float height;
   float angle;
   float angularVelocity;
   float inertia;
    float velocityX;
    float velocityY;

    float mass;

    PhysicsObjectRect(
        float posx,
        float posy,
        float velY,
        float velX,
        float w,
        float h,float Q,float W);

         void update(float deltaTime);
};

#endif