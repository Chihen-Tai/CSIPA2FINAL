#include "ball.h"
#include "../data/DataCenter.h"
#include "../data/imageCenter.h"
#include "../shapes/Circle.h"
#include "../shapes/Rectangle.h"
#include "../shapes/Point.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "../data/SoundCenter.h"
#include <iostream>
#include <vector>
#include <random>
std::random_device rd;                          // Only used once to initialise (seed) engine
std::mt19937 rng(rd());                         // Random-number engine used (Mersenne-Twister in this case)
std::uniform_int_distribution<int> uni(0, 255); // Guaranteed unbiased
namespace BallSetting
{
    static constexpr char png_root_path[40] = {
        "./assets/image/ball/IDR_GIF3.png",
    };
};
Ball *Ball::create_ball()
{

    srand((unsigned)time(NULL));
    Ball *ball = new Ball();
    DataCenter *DC = DataCenter::get_instance();
    ball->shape.reset(new Circle{DC->window_width / 2, DC->window_height / 2, 50});
    ball->r = uni(rng);
    ball->g = uni(rng);
    ball->b = uni(rng);
    if (ball->r % 2 == 0)
    {
        ball->vx = abs(ball->vx);
    }
    else
    {
        ball->vx = -abs(ball->vx);
    }
    if (ball->b % 3 == 0)
    {
        ball->vy = abs(ball->vy);
    }
    else
    {
        ball->vy = -abs(ball->vy);
    }
    return ball;
}
void Ball::init()
{
    std::cout << "Ball init" << std::endl;
    DataCenter *DC = DataCenter::get_instance();
    std::cout << "Ball init2" << std::endl;
    // ImageCenter *IC = ImageCenter::get_instance();
    shape.reset(new Circle{DC->window_width / 2, DC->window_height / 2, 50});
    std::cout << "Ball init3" << std::endl;
    // std::cout<<random<<std::endl;
}
constexpr char sound_path[] = "./assets/sound/sound1.wav";
void Ball::update()
{
    SoundCenter *SC = SoundCenter::get_instance();
    shape->update_center_x(shape->center_x() + vx);
    shape->update_center_y(shape->center_y() - vy);
    // SC->play(sound_path, ALLEGRO_PLAYMODE_ONCE);
    bool play_sound = false;
    std::vector<Ball *> &objects = DataCenter::get_instance()->balls;
    for (size_t i = 0; i < objects.size(); ++i)
    {

        Ball *obj1 = objects[i];
        for (size_t j = i + 1; j < objects.size(); ++j)
        {
            Ball *obj2 = objects[j];
            double dx = obj1->getX() - obj2->getX();
            double dy = obj1->getY() - obj2->getY();
            double dr = 30;
            if (dx * dx + dy * dy < dr * dr)
            {
                double w1 = 1.0;
                double w2 = 1.0;
                Vector v1(obj1->getSpeedX(), obj1->getSpeedY());
                Vector v2(obj2->getSpeedX(), obj2->getSpeedY());
                Vector dv = v1 - v2;
                Vector dp = Vector(dx, dy);
                Vector u1 = v1 - ((2 * w2 / (w1 + w2)) * (dv * dp / (~dp * ~dp))) * dp;
                Vector u2 = v2 - ((2 * w1 / (w1 + w2)) * ((-dv) * (-dp) / (~dp * ~dp))) * (-dp);
                obj1->vx = u1.getX();
                obj1->vy = u1.getY();
                obj2->vx = u2.getX();
                obj2->vy = u2.getY();
                // fix overlapping
                double d = sqrt(dx * dx + dy * dy);
                double overlap = dr - d;
                double overlapX = overlap * dx / d;
                double overlapY = overlap * dy / d;
                obj1->setX(obj1->getX() + overlapX / 2);
                obj1->setY(obj1->getY() + overlapY / 2);
                obj2->setX(obj2->getX() - overlapX / 2);
                obj2->setY(obj2->getY() - overlapY / 2);
                // play_sound = true;
            }
        }
    }
    if (shape->center_x() - 15 < 0)
    {
        shape->update_center_x(15);
        vx = -vx;
        play_sound = true;
    }
    else if (shape->center_x() + 15 > 1280)
    {
        shape->update_center_x(1280 - 15);
        vx = -vx;
        play_sound = true;
    }
    if (shape->center_y() - 15 < 0)
    {
        shape->update_center_y(15);
        vy = -vy;
        play_sound = true;
    }
    else if (shape->center_y() + 15 > 800)
    {
        shape->update_center_y(800 - 15);
        vy = -vy;
        play_sound = true;
    }

    if (play_sound)
    {
        for (int i = 0; i < 10; i++)
        {
            SC->play(sound_path, ALLEGRO_PLAYMODE_ONCE);
        }
        // std::cout<<"play sound"<<std::endl;
    }
}

void Ball::draw()
{
    // ImageCenter *IC = ImageCenter::get_instance();
    // ALLEGRO_BITMAP *bitmap = IC->get(BallSetting::png_root_path);

    // Draw the ball with color
    al_draw_filled_circle(
        shape->center_x(),
        shape->center_y(),
        15,
        al_map_rgb(r, g, b) // Red color
    );

    // Draw the bitmap on top of the colored ball
    // al_draw_bitmap(
    //     bitmap,
    //     shape->center_x() - al_get_bitmap_width(bitmap) / 2,
    //     shape->center_y() - al_get_bitmap_height(bitmap) / 2, 0);
}