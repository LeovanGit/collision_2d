#include "../inc/physics.hpp"

extern float delta_time;

Circle::Circle(float x, float y, 
               float speed_x, float speed_y,
               float acceleration_x, float acceleration_y) :
               x(x), y(y),
               speed_x(speed_x), speed_y(speed_y),
               acceleration_x(acceleration_x), acceleration_y(acceleration_y)
               {}

float Circle::getX() { return this->x; }
float Circle::getY() { return this->y; }

void Circle::move()
{
    x += speed_x * delta_time;
    y += speed_y * delta_time;

    speed_x += acceleration_x * delta_time;
    speed_y += acceleration_y * delta_time;

    if (x > 1.0f || x < -1.0f) speed_x = -speed_x;
    if (y > 1.0f || y < -1.0f) speed_y = -speed_y;
}
