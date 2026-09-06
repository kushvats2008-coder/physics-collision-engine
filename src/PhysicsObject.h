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
    PhysicsObject(float posx, float posy, float velY, float velX, float accX, float accY, float f, float r);

    void update(float deltaTime);
};

#endif