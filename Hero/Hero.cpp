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
    DataCenter *DC=DataCenter::get_instance();
    if(DC->key_state[ALLEGRO_KEY_W])
    {
        shape->update_center_y(shape->center_y() - speed);
        state = HeroState::BACK;
    }
    else if(DC->key_state[ALLEGRO_KEY_S])
    {
        shape->update_center_y(shape->center_y() + speed);
        state = HeroState::FRONT;
    }
    else if(DC->key_state[ALLEGRO_KEY_A])
    {
        shape->update_center_x(shape->center_x() - speed);
        state = HeroState::LEFT;
    }
    else if(DC->key_state[ALLEGRO_KEY_D])
    {
        shape->update_center_x(shape->center_x() + speed);
        state = HeroState::RIGHT;
    }
}

Hero_Bullet *Hero::create_bullet(Object *target)
{
    DataCenter *DC = DataCenter::get_instance();
    const Point &p = Point(shape->center_x(), shape->center_y());
    const Point &t = DC->mouse;
    return new Hero_Bullet(p, t, HeroSetting::hero_bullet_img_path, 10, 10, 100);
}
bool Hero::attack(Object *target)
{
    SoundCenter *SC = SoundCenter::get_instance();
    SC->play(HeroSetting::attack_sound_path, ALLEGRO_PLAYMODE_ONCE);
    DataCenter *DC = DataCenter::get_instance();
    DC->heroBullets.emplace_back(create_bullet(target));
    counter = attack_freq;
    return true;
}
