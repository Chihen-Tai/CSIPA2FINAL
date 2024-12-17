#include "block.h"
#include "../data/DataCenter.h"
#include "../data/FontCenter.h"
#include "../data/imageCenter.h"
#include "../shapes/Rectangle.h"
#include "../shapes/Point.h"
#include <iostream>
#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "../Hero/Hero_Bullet.h"
#include "../ball/ball.h"
#include "../Level.h"
#include "../Player.h"

Block *Block::create_block(int i,int j,int hp)
{
    
    Block *block = new Block();
    block->i=i;
    block->j=j;
    block->hp=hp;    
    block->shape.reset(new Rectangle{
        (double)(x_init+i*(block->width+space)),
        (double)(y_init+j*(block->height+space)),
        (double)(x_init+i*(block->width+space)+block->width),
        (double)(y_init+j*(block->height+space)+block->height)
    });
    return block;
}

void Block::update()
{
    if(hp<=0)
    {
        if(exist==true)
        {
            DataCenter::get_instance()->level->total_blocks--;  
            exist=false;
        }
    }
    DataCenter *DC = DataCenter::get_instance();
    std::vector<Hero_Bullet*> &heroBullets = DC->heroBullets;
    for(size_t i=0;i<heroBullets.size();++i)
    {
        if(heroBullets[i]->shape->overlap(*(this->shape)) && exist)
        {
            DC->player->coin+=hp;
            hp=0;
            delete heroBullets[i];
            heroBullets.erase(heroBullets.begin()+i);
            --i;
        }
    }
    // for(size_t i=0;i<DC->balls.size();++i)
    // {
    //     if(DC->balls[i]->shape->overlap(*(shape)) && exist)
    //     {
    //         hp-=DC->balls[i]->getDamage();
    //         --i;
    //     }
    // }
}


void Block::draw()
{
    if(!exist)
    {
        return;
    }
    al_draw_filled_rectangle(
    x_init+i*(width+space),
    y_init+j*(height+space),
    x_init+i*(width+space)+width,
    y_init+j*(height+space)+height,
    al_map_rgb((hp*34298)%255,(hp*232456)%255,(hp*9122)%255));
    //DataCenter *DC = DataCenter::get_instance();
    FontCenter *FC = FontCenter::get_instance();
    std::string s = std::to_string(hp);
    al_draw_text(
        FC->caviar_dreams[FontSize::MEDIUM], al_map_rgb(0, 0, 0),
        x_init + i * (width + space) + width / 2-1,
        y_init + j * (height + space) + height / 2-12,
        ALLEGRO_ALIGN_CENTRE, s.c_str());
}