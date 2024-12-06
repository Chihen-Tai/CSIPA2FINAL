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
    Normal,
    BIG
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
    static Ball* create_ball(BallState state);
    double getX() { return shape->center_x(); }
    double getY() { return shape->center_y(); }   
    const double &getSpeedX() const { return vx; }
    const double &getSpeedY() const { return vy; } 
    void setX(double x) { shape->update_center_x(x); }
    void setY(double y) { shape->update_center_y(y); }
    const double &getRadius() const { return radius; }
    const double &getWeight() const { return weight; }
    const int &getDamage() const { return damage; }
    const double &get_vx() const { return vx; }
    const double &get_vy() const { return vy; }
    void set_vx(double vx) { this->vx = vx; }
    void set_vy(double vy) { this->vy = vy; }
    void setDamage(int damage) { this->damage = damage; }
    static int buy_damage;
    static double speed;
    private:
    std::map<BallState,std::string>  pngpath;
    
    int damage;
    double vx;
    double vy;
    int r;
    int g;
    int b;
    double weight;
    double radius;
};



#endif // BALL_H_INCLUDED