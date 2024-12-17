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
#include "../block/block.h"
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
    constexpr char attack_sound_path[] = "./assets/sound/shoot2.mp3";
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
    GIFCenter *GIFC = GIFCenter::get_instance();
    //DataCenter *DC = DataCenter::get_instance();
    // see Monster.cpp 148
    ALGIF_ANIMATION *gif = GIFC->get(gifPath[HeroState::FRONT]);
    shape.reset(new Rectangle{
        1280-gif->width,
        800,
        gif->width,
        gif->height});
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
void Hero::update()
{
    DataCenter *DC = DataCenter::get_instance();
    SoundCenter *SC = SoundCenter::get_instance();
    if (DC->key_state[ALLEGRO_KEY_W] )
    {
        shape->update_center_y(shape->center_y() - speed);
        state = HeroState::BACK;
        
    }
    else if (DC->key_state[ALLEGRO_KEY_S] )
    {
        shape->update_center_y(shape->center_y() + speed);
        state = HeroState::FRONT;
        
    }
    else if (DC->key_state[ALLEGRO_KEY_A] )
    {
        shape->update_center_x(shape->center_x() - speed);
        state = HeroState::LEFT;
        
    }
    else if (DC->key_state[ALLEGRO_KEY_D] )
    {
        shape->update_center_x(shape->center_x() + speed);
        state = HeroState::RIGHT;
       
    }
    if(shape->center_x() < 0)
    {
        shape->update_center_x(DC->window_width);
    }
    if(shape->center_x() > DC->window_width)
    {
        shape->update_center_x(0);
    }
    if(shape->center_y() < 0)
    {
        shape->update_center_y(DC->window_height);
    }
    if(shape->center_y() > DC->window_height)
    {
        shape->update_center_y(0);
    }
    if (counter > 0)
    {
        counter--;
    }
    if (counter == 0)
    {
    for(int i=0;i<5;i++)
    {
        SC->play(HeroSetting::attack_sound_path, ALLEGRO_PLAYMODE_ONCE);
    }
    attack(nullptr); // First shot
    counter = 20; // Reset counter for 3 seconds delay (assuming 60 FPS)
    }
}


Hero_Bullet *Hero::create_bullet(Object *target)
{
    DataCenter *DC = DataCenter::get_instance();
    const Point &p = Point(shape->center_x(), shape->center_y());
    const Point &t = DC->mouse;
    return new Hero_Bullet(p, t, HeroSetting::hero_bullet_img_path);
}

bool Hero::attack(Object *target)
{
    DataCenter *DC = DataCenter::get_instance();
    DC->heroBullets.emplace_back(create_bullet(target));
    return true;
}
