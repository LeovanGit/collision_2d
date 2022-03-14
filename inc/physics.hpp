#ifndef PHYSICS_HPP
#define PHYSICS_HPP

class Circle
{
public:
    Circle(float x, float y, 
           float speed_x, float speed_y,
           float acceleration_x, float acceleration_y);

    float getX();
    float getY();

    void move();
    
private:
    float x, y;
    float speed_x, speed_y;
    float acceleration_x, acceleration_y;
};

#endif

