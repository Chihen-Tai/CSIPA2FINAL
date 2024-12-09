#include "Hero_Bullet.h"
#include "../data/DataCenter.h"
#include "../data/ImageCenter.h"
#include "../shapes/Circle.h"
#include "../shapes/Point.h"
#include <algorithm>
#include <allegro5/bitmap_draw.h>

Hero_Bullet::Hero_Bullet(const Point &p, const Point &target, const std::string &path)
{
	ImageCenter *IC = ImageCenter::get_instance();
	this->fly_dist = get_fly_dist();
	this->v=get_v();
	bitmap = IC->get(path);
	double r = std::min(al_get_bitmap_width(bitmap), al_get_bitmap_height(bitmap)) * 0.2;
	shape.reset(new Circle{p.x, p.y, r});
	double d = Point::dist(p, target);
	vx = (target.x - p.x) / d * v;
	vy = (target.y - p.y) / d * v;
	// std::cout<<"vx: "<<vx<<std::endl;
	// std::cout<<"vy: "<<vy<<std::endl;
}
void
Hero_Bullet::update() {
	DataCenter *DC = DataCenter::get_instance();
	double dx = vx / DC->FPS;
	double dy = vy / DC->FPS;
	shape->update_center_x(shape->center_x() + dx);
	shape->update_center_y(shape->center_y() + dy);
	//std::cout<<"update"<<std::endl;
}
void
Hero_Bullet::draw() {
	
	al_draw_bitmap(
		bitmap,
		shape->center_x() - al_get_bitmap_width(bitmap) / 2,
		shape->center_y() - al_get_bitmap_height(bitmap) / 2, 0);
		//std::cout<<"draw"<<std::endl;
}