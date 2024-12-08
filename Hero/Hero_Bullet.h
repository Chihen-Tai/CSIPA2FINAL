#ifndef HEROBULLET_H_INCLUDED
#define HEROBULLET_H_INCLUDED

#include "../Object.h"
#include <allegro5/bitmap.h>
#include <string>
#include <iostream>

class Hero_Bullet : public Object
{
public:
	Hero_Bullet(const Point &p,const Point &target, const std::string &path);
	void update();
	void draw();
	int get_dmg() const { return dmg; }
	//virtual bool attack(Object *target) { return false; }
	double get_fly_dist() const { return fly_dist; }
	double get_v() const { return v; }
private:
	ALLEGRO_BITMAP *bitmap;
	double vx, vy;
	double v=150;
	double fly_dist=10;
	int dmg=1;
};


#endif