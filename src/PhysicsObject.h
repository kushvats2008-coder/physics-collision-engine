#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H

class PhysicsObject
{
public:
    static constexpr float defaultRadius = 30.0f;
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
    float resitutionX;
    float resitutionY;
    float width;
    float height;
    float radius = defaultRadius;
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
    bool isstatic = false;
    float mass;

    PhysicsObjectRect(
        float posx,
        float posy,
        float velY,
        float velX,
        float w,
        float h,float Q,float W,bool staticBody = false);

         void update(float deltaTime);
};

#endif