#ifndef HERO_H_INCLUDED
#define HERO_H_INCLUDED

#include "../Object.h"
#include <map>
#include <string>
#include <iostream>
#include <allegro5/allegro.h>
#include "Hero_Bullet.h"

enum class HeroState
{
    LEFT, 
    RIGHT, 
    FRONT,
    BACK,
    HEROSTATE_MAX
};

class Hero : public Object
{
public:
    void update();
    void draw();
    void move();
    void init();
    bool attack(Object *target);
    Hero_Bullet *create_bullet(Object *target);

private:
    HeroState state = HeroState::FRONT;
    double speed = 5.0;
    std::map<HeroState,std::string> gifPath;
    int attack_freq = 10 ;
    int counter = 18;
};   

#endif // HERO_H_INCLUDED