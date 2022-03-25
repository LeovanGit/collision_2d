#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include "vector2.hpp"

class Ball
{
public:
    Ball(float weight,
         float radius,
         Vec2 position,
         Vec2 speed,
         Vec2 acceleration);

    Ball(float weight,
         float radius,
         float x, float y, 
         float speed_x, float speed_y,
         float acceleration_x, float acceleration_y);

    void setPosition(Vec2 position);

    void setSpeed(Vec2 speed);

    Vec2 getPosition() const;

    Vec2 getSpeed() const;

    float getRadius() const;
    
    float getWeight() const;

    bool checkCollision(const Ball & other);

    void move();
    
private:
    float weight;
    float radius;
    Vec2 position;
    Vec2 speed;
    Vec2 acceleration;
};

void calculateSpeedAfterCollision(Ball & c1, Ball & c2);

#endif

