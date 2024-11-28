#include "ball.h"
#include "../data/DataCenter.h"
#include "../data/imageCenter.h"
#include "../shapes/Circle.h"
#include "../shapes/Rectangle.h"
#include "../shapes/Point.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <iostream>

namespace BallSetting
{
    static constexpr char png_root_path[40] = {
        "./assets/image/ball/IDR_GIF3.png",
    };
};

void Ball::init()
{
    std::cout << "Ball init" << std::endl;
    DataCenter *DC = DataCenter::get_instance();
    std::cout << "Ball init2" << std::endl;
    //ImageCenter *IC = ImageCenter::get_instance();
    shape.reset(new Circle{DC->window_width / 2, DC->window_height / 2, 50});
    std::cout << "Ball init3" << std::endl;
}

void Ball::update()
{
    //DataCenter *DC = DataCenter::get_instance();
    // Move the ball in a straight line
    shape->update_center_x(shape->center_x() + 0.1);
    shape->update_center_y(shape->center_y() - 0.1);
}

void Ball::draw()
{
    //ImageCenter *IC = ImageCenter::get_instance();
    //ALLEGRO_BITMAP *bitmap = IC->get(BallSetting::png_root_path);
    
    // Draw the ball with color
    al_draw_filled_circle(
        shape->center_x(),
        shape->center_y(),
        25,
        al_map_rgb(255, 0, 0) // Red color
    );

    // Draw the bitmap on top of the colored ball
    // al_draw_bitmap(
    //     bitmap,
    //     shape->center_x() - al_get_bitmap_width(bitmap) / 2,
    //     shape->center_y() - al_get_bitmap_height(bitmap) / 2, 0);
}