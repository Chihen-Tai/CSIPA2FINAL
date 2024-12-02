#include "Hero.h"
#include <cstdio>
#include "../data/DataCenter.h"
#include "../data/GIFCenter.h"
#include "../algif5/algif.h"
#include "../shapes/Rectangle.h"
#include "../data/SoundCenter.h"
#include "../shapes/Point.h"
#include "../shapes/Circle.h"
#include "Hero_Bullet.h"
#include "../data/ImageCenter.h"
#include "../ball/Ball.h"
namespace HeroSetting
{
    static constexpr char gif_root_path[40] = {
        "./assets/gif/Hero",
    };
    static constexpr char gid_postfix[][10] = {
        "left", "right", "front", "back"};
    static constexpr char hero_bullet_img_path[] = {
        "./assets/image/tower/Arcane_Beam.png",
    };
    constexpr char attack_sound_path[] = "./assets/sound/Arrow.wav";
}
void Hero::init()
{
    for (size_t type = 0; type < static_cast<size_t>(HeroState::HEROSTATE_MAX); ++type)
    {
        char buffer[50];
        sprintf(
            buffer, "%s/dragonite_%s.gif", // print the string to buffer
            HeroSetting::gif_root_path,
            HeroSetting::gid_postfix[static_cast<int>(type)]);
        gifPath[static_cast<HeroState>(type)] = std::string(buffer);
    }
    DataCenter *DC = DataCenter::get_instance();
    GIFCenter *GIFC = GIFCenter::get_instance();
    // see Monster.cpp 148
    ALGIF_ANIMATION *gif = GIFC->get(gifPath[HeroState::FRONT]);
    shape.reset(new Rectangle{
        DC->window_width / 2,
        DC->window_height / 2,
        DC->window_width / 2 + gif->width,
        DC->window_height / 2 + gif->height});
    for(int i=0;i<70;i++)
    {
        DC->balls.emplace_back(Ball::create_ball());
    }
}
void Hero::draw()
{
    GIFCenter *GIFC = GIFCenter::get_instance();
    ALGIF_ANIMATION *gif = GIFC->get(gifPath[state]);
    algif_draw_gif(
        gif,
        shape->center_x() - gif->width / 2,
        shape->center_y() - gif->height / 2, 0);
} 
// double top()
// {   
//     GIFCenter *GIFC = GIFCenter::get_instance();
//     ALGIF_ANIMATION *gif = GIFC->get(gifPath[state]);
//     return shape->center_y() - gif->height() / 2;
// }
// double bottom()
// {
//     return shape->center_y() + gif->height() / 2;
// }
// double left()
// {
//     return shape->center_x() - gif->width() / 2;
// }
// double right()
// {
//     return shape->center_x() + gif->width() / 2;
// }
void Hero::update()
{
    DataCenter *DC = DataCenter::get_instance();
    // std::cout <<"top"<<shape->top()<<std::endl;
    // std::cout <<"bottom"<<shape->bottom()<<std::endl;
    // std::cout <<"left"<<shape->left()<<std::endl;
    // std::cout <<"right"<<shape->right()<<std::endl;
    // std::cout <<"window_height"<<DC->window_height<<std::endl;
    // std::cout <<"window_width"<<DC->window_width<<std::endl;
    if (DC->key_state[ALLEGRO_KEY_W] && shape->center_x()<1200)
    {
        shape->update_center_y(shape->center_y() - speed);
        state = HeroState::BACK;
        
    }
    else if (DC->key_state[ALLEGRO_KEY_S] &&shape->center_x() >0)
    {
        shape->update_center_y(shape->center_y() + speed);
        state = HeroState::FRONT;
        
    }
    else if (DC->key_state[ALLEGRO_KEY_A] && shape->center_y() < 800)
    {
        shape->update_center_x(shape->center_x() - speed);
        state = HeroState::LEFT;
        
    }
    else if (DC->key_state[ALLEGRO_KEY_D] && shape->center_y() >0)
    {
        shape->update_center_x(shape->center_x() + speed);
        state = HeroState::RIGHT;
       
    }
    if (counter > 0)
    {
        counter--;
    }
    if (counter == 0)
    {
        attack(nullptr); // 3 seconds delay (assuming 60 FPS)
    }
}

Hero_Bullet *Hero::create_bullet(Object *target)
{
    DataCenter *DC = DataCenter::get_instance();
    const Point &p = Point(shape->center_x(), shape->center_y());
    const Point &t = DC->mouse;
    return new Hero_Bullet(p, t, HeroSetting::hero_bullet_img_path, 150, 1, 100000);
}
bool Hero::attack(Object *target)
{
    //std::cout<<"attack"<<std::endl;
    SoundCenter *SC = SoundCenter::get_instance();
    SC->play(HeroSetting::attack_sound_path, ALLEGRO_PLAYMODE_ONCE);
    DataCenter *DC = DataCenter::get_instance();
    DC->heroBullets.emplace_back(create_bullet(target));
    counter = attack_freq;
    return true;
}
