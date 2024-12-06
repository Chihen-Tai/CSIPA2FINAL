#ifndef BLOCK_H_INCLUDED
#define BLOCK_H_INCLUDED

#include <string>
#include <map>
#include "../Object.h"
#include <random>

class Block : public Object
{
    public:
    void update();
    void draw();
    static Block *create_block(int i,int j,int hp);
    const int get_hp() const { return hp; }
    void set_hp(int hp) { this->hp = hp; }
    const int get_width() const { return width; }
    const int get_height() const { return height; }
    const int getX() const { return x_init+i*(width+space); }
    const int getY() const { return y_init+j*(height+space); }
    private:
    int hp=2; 
    int i,j;
    const static int space = 1;
    int r, g, b;
    const static int x_init=128;
    const static int y_init=80;
    int width=60, height=32;
    bool exist=true;
    int map_data[20][20];
};

#endif
