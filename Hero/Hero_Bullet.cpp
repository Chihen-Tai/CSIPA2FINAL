#include "Hero_Bullet.h"
#include "../data/DataCenter.h"
#include "../data/ImageCenter.h"
#include "../shapes/Circle.h"
#include "../shapes/Point.h"
#include <algorithm>
#include <allegro5/bitmap_draw.h>

Hero_Bullet::Hero_Bullet(const Point &p, const Point &target, const std::string &path, double v, int dmg, double fly_dist )
{
	ImageCenter *IC = ImageCenter::get_instance();
	this->fly_dist = 100;
	this->dmg = 10;
	bitmap = IC->get(path);
	double r = std::min(al_get_bitmap_width(bitmap), al_get_bitmap_height(bitmap)) * 0.8;
	shape.reset(new Circle{p.x, p.y, r});
	vx = 10000;
	vy = 10000;
}
void
Hero_Bullet::update() {
	DataCenter *DC = DataCenter::get_instance();
	double dx = vx / DC->FPS;
	double dy = vy / DC->FPS;
	shape->update_center_x(shape->center_x() + dx);
	shape->update_center_y(shape->center_y() + dy);
}
void
Hero_Bullet::draw() {
	al_draw_bitmap(
		bitmap,
		shape->center_x() - al_get_bitmap_width(bitmap) / 2,
		shape->center_y() - al_get_bitmap_height(bitmap) / 2, 0);
}