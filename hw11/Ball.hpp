#pragma once
#include "Painter.hpp"
#include "Point.hpp"
#include "Velocity.hpp"

class Ball {
    Point center;
    Velocity velocity;
    Color color;
    double radius;
    double mass;
public:
    Ball(Point point, Velocity velocity, Color color, double radius, double mass);
    void setVelocity(const Velocity& velocity);
    Velocity getVelocity() const;
    void draw(Painter& painter) const;
    void setCenter(const Point& center);
    Point getCenter() const;
    double getRadius() const;
    double getMass() const;
    void setColor(const Color& color);
};
