#ifndef HEROBULLET_H_INCLUDED
#define HEROBULLET_H_INCLUDED

#include "../Object.h"
#include <allegro5/bitmap.h>
#include <string>
#include <iostream>

class Hero_Bullet : public Object
{
public:
	Hero_Bullet(const Point &p,const Point &target, const std::string &path, double v, int dmg, double fly_dist);
	void update();
	void draw();
	int get_dmg() const { return dmg; }
	//virtual bool attack(Object *target) { return false; }
	
private:
	ALLEGRO_BITMAP *bitmap;
	double vx, vy;
	double fly_dist;
	int dmg=1;
};


#endif