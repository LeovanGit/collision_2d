#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include <cmath>

class Vec2
{
public:
    explicit Vec2(float value = 0);

    Vec2(float x, float y);

    Vec2(const Vec2 & other);

    // distance between vectors coords
    float distance(const Vec2 & other);
    
    void setX(float value);
    void setY(float value);

    float getX() const;
    float getY() const;

    Vec2 & operator=(const Vec2 & other) 
    {
        this->x = other.x;
        this->y = other.y;
        return *this;
    }
    
    float getLength() const;

    Vec2 normalize() { return Vec2(x / getLength(), y / getLength()); }

    Vec2 operator*(const Vec2 & other) { return Vec2(x * other.x, y * other.y); }

    Vec2 operator*(float value) { return Vec2(x * value, y * value); }

    Vec2 operator/(float value) { return Vec2(x / value, y / value); }

    Vec2 operator+(const Vec2 & other) { return Vec2(x + other.x, y + other.y); }

    Vec2 operator-(const Vec2 & other) { return Vec2(x - other.x, y - other.y); }

    Vec2 & operator+=(const Vec2 & other) 
    {
        x = x + other.x;
        y = y + other.y; 
        return *this;
    }

    Vec2 & operator-=(const Vec2 & other) 
    {
        x = x - other.x;
        y = y - other.y;
        return *this;
    }

    Vec2 operator-() { return Vec2(-x, -y); }

private:
    float x;
    float y;
};

float dot(const Vec2 & a, const Vec2 & b);
Vec2 sum(const Vec2 & a, const Vec2 & b);
Vec2 sub(const Vec2 & a, const Vec2 & b);

#endif
