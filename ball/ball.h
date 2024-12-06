#ifndef BALL_H_INCLUDED
#define BALL_H_INCLUDED

#include <string>
#include <map>
#include "../Object.h"
#include <random>
#include <type_traits>
#include <time.h>
enum class BallState
{
    Normal
};
class Vector
{
public:
    Vector(double X, double Y) : x{X}, y{Y} {}
    double &getX() { return x; }
    double &getY() { return y; }
    const double &getX() const { return x; }
    const double &getY() const { return y; }
    double operator*(const Vector &rhs) const
    {
        return x * rhs.x + y * rhs.y;
    }
    Vector operator*(const double &rhs) const
    {
        return Vector(x * rhs, y * rhs);
    }
    friend Vector operator*(double lhs, const Vector &rhs)
    {
        return Vector(lhs * rhs.x, lhs * rhs.y);
    }
    Vector operator+(const Vector &rhs) const
    {
        return Vector(x + rhs.x, y + rhs.y);
    }
    Vector operator-(const Vector &rhs) const
    {
        return Vector(x - rhs.x, y - rhs.y);
    }
    double operator~() const
    {
        return sqrt(x * x + y * y);
    }
    // Vector v; (~v) =? length of v;
    Vector operator-() const
    {
        return Vector(-x, -y);
    }

private:
    double x, y;
};
class Ball : public Object
{
    public:
    void init();    
    void update();
    void draw();
    static Ball* create_ball();
    double getX() { return shape->center_x(); }
    double getY() { return shape->center_y(); }   
    const double &getSpeedX() const { return vx; }
    const double &getSpeedY() const { return vy; } 
    void setX(double x) { shape->update_center_x(x); }
    void setY(double y) { shape->update_center_y(y); }
    const double &getRadius() const { return radius; }
    const double &getWeight() const { return weight; }
    const int &getDamage() const { return damage; }
    private:
    std::map<BallState,std::string>  pngpath;
    double vx;
    double vy;
    int damage;
    int r;
    int g;
    int b;
    double weight;
    double radius;
};

#endif // BALL_H_INCLUDED