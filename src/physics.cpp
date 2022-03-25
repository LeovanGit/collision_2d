#include "physics.hpp"
#include <cmath>

extern float delta_time;
extern float aspect;

static float x_lim = 1.0f;
static float y_lim = 1.0f;

Ball::Ball(float weight,
           float radius,
           Vec2 position,
           Vec2 speed,
           Vec2 acceleration) :
           weight(weight),
           radius(radius),
           position(position),
           speed(speed),
           acceleration(acceleration)
           {}

Ball::Ball(float weight,
           float radius,
           float x, float y, 
           float speed_x, float speed_y,
           float acceleration_x, float acceleration_y) :
           weight(weight),
           radius(radius),
           position(Vec2(x, y)),
           speed(Vec2(speed_x, speed_y)),
           acceleration(Vec2(acceleration_x, acceleration_y))
           {}

void Ball::setPosition(Vec2 position) { this->position = position; }

void Ball::setSpeed(Vec2 speed) { this->speed = speed; }

Vec2 Ball::getPosition() const { return this->position; }

Vec2 Ball::getSpeed() const { return this->speed; }

float Ball::getRadius() const { return this->radius; }

float Ball::getWeight() const { return this->weight; }

bool Ball::checkCollision(const Ball & other)
{
    return position.distance(other.position) <= other.radius + this->radius;
}

void Ball::move()
{
    position += speed * delta_time;

    speed += acceleration * delta_time;

    float x = position.getX();
    if (x + radius >= x_lim)
    {
        float delta = x + radius - x_lim;
        position.setX(x - 2 * delta);
        speed.setX(-speed.getX());
    }
    else if (x - radius <= -x_lim)
    {
        float delta = x - radius + x_lim;
        position.setX(x - 2 * delta);
        speed.setX(-speed.getX());
    }

    float y = position.getY();
    if (y + radius >= y_lim * aspect)
    {
        float delta = y + radius - y_lim * aspect;
        position.setY(y - 2 * delta);
        speed.setY(-speed.getY());
    }
    else if (y - radius <= -y_lim * aspect)
    {
        float delta = y - radius + y_lim * aspect;
        position.setY(y - 2 * delta);
        speed.setY(-speed.getY());
    }
}

void calculateSpeedAfterCollision(Ball & c1, Ball & c2)
{
    float m1 = c1.getWeight();
    float m2 = c2.getWeight();

    Vec2 p1 = c1.getPosition();
    Vec2 p2 = c2.getPosition();
    
    Vec2 v1 = c1.getSpeed();
    Vec2 v2 = c2.getSpeed();

    // new y-axis through centers
    Vec2 y = (p2 - p1).normalize();

    // y-axis speed component
    Vec2 v1_y = y * (dot(v1, y) / y.getLength());
    Vec2 v2_y = y * (dot(v2, y) / y.getLength());

    // system of laws of momentum and kinetic energy conservation for y-axis
    Vec2 u1_y = y * ((dot(v1, y) * (m1 - m2) + 2 * m2 * dot(v2, y)) / (m1 + m2));
    Vec2 u2_y = u1_y + v1_y - v2_y;

    Vec2 new_v1 = sum(sub(v1, v1_y), u1_y);
    Vec2 new_v2 = sum(sub(v2, v2_y), u2_y);

    c1.setSpeed(new_v1);
    c2.setSpeed(new_v2);    

    // against the tunneling effect
    float delta = c1.getRadius() + c2.getRadius() - p1.distance(p2);
    c2.setPosition(p2 + y * delta);
}
