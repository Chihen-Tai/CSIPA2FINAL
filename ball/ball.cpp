#include "ball.h"
#include "../data/DataCenter.h"
#include "../data/imageCenter.h"
#include "../shapes/Circle.h"
#include "../shapes/Rectangle.h"
#include "../shapes/Point.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "../data/SoundCenter.h"
#include "../block/block.h"
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
    ball->weight = 1;
    ball->radius = 15;
    ball->vx = 5;
    ball->vy = 5;
    ball->damage = 1;
    DataCenter *DC = DataCenter::get_instance();
    ball->r = uni(rng);
    ball->g = uni(rng);
    ball->b = uni(rng);
    int startX;
    int startY;
    if (ball->r % 2 == 0)
    {
        ball->vx = abs(ball->vx);
        startX = 50;
    }
    else
    {
        ball->vx = -abs(ball->vx);
        startX = 1230;
    }
    if (ball->b % 3 == 0)
    {
        ball->vy = abs(ball->vy);
        startY = 50;
    }
    else
    {
        ball->vy = -abs(ball->vy);
        startY = 750;
    }
    ball->shape.reset(new Circle{
        (double)startX,
        (double)startY,
        (double)ball->radius});
    return ball;
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
            double dr = obj1->getRadius() + obj2->getRadius();
            if (dx * dx + dy * dy < dr * dr)
            {
                double w1 = obj1->getWeight();
                double w2 = obj2->getWeight();
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
    std::vector<Block *> &blocks = DataCenter::get_instance()->blocks;
    for (Block *block : blocks)
    {
        double dx = shape->center_x() - block->getX();
        double dy = shape->center_y() - block->getY();
        double half_block_width = block->get_width() / 2;
        double half_block_height = block->get_height() / 2;

        // 检测是否发生碰撞
        if (abs(dx) < half_block_width + radius && abs(dy) < half_block_height + radius)
        {
            // 确定碰撞方向
            double overlapX = half_block_width + radius - abs(dx);
            double overlapY = half_block_height + radius - abs(dy);

            if (overlapX < overlapY)
            {
                // 水平方向碰撞
                if (dx > 0) // 球在方块的左边
                    shape->update_center_x(block->getX() - half_block_width - radius);
                else // 球在方块的右边
                    shape->update_center_x(block->getX() + half_block_width + radius);

                vx = -vx; // 反转水平速度
            }
            else
            {
                // 垂直方向碰撞
                if (dy > 0) // 球在方块的上边
                    shape->update_center_y(block->getY() - half_block_height - radius);
                else // 球在方块的下边
                    shape->update_center_y(block->getY() + half_block_height + radius);

                vy = -vy; // 反转垂直速度
            }

            // 播放声音
            play_sound = true;
        }
    }

    if (play_sound)
    {
        SC->play(sound_path, ALLEGRO_PLAYMODE_ONCE);
        // std::cout<<"play sound"<<std::endl;
    }
}

void Ball::draw()
{

    al_draw_filled_circle(
        shape->center_x(),
        shape->center_y(),
        15,
        al_map_rgb(r, g, b) // Red color
    );
}