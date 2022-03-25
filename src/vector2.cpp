#include "vector2.hpp"

Vec2::Vec2(float value) : x(value), y(value) {};

Vec2::Vec2(float x, float y) : x(x), y(y) {};

Vec2::Vec2(const Vec2 & other) : x(other.x), y(other.y) {};

float Vec2::distance(const Vec2 & other)
{
    return sqrt((x - other.getX()) * (x - other.getX()) +
                (y - other.getY()) * (y - other.getY()));
}

void Vec2::setX(float value) { this->x = value; }
void Vec2::setY(float value) { this->y = value; }

float Vec2::getX() const { return this->x; }
float Vec2::getY() const { return this->y; }

float Vec2::getLength() const { return sqrt(x * x + y * y); }

Vec2 normalize();

float dot(const Vec2 & a, const Vec2 & b)
{
    return a.getX() * b.getX() + a.getY() * b.getY();
}

Vec2 sum(const Vec2 & a, const Vec2 & b)
{
    return Vec2(a.getX() + b.getX(), a.getY() + b.getY());
}

Vec2 sub(const Vec2 & a, const Vec2 & b)
{
    return Vec2(a.getX() - b.getX(), a.getY() - b.getY());
}
