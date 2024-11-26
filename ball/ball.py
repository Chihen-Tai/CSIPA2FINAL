#ifndef BALL_H_INCLUDED
#define BALL_H_INCLUDED

#include "../Object.h"
#include "../shapes/Circle.h"
#include <vector>
#include <allegro5/bitmap.h>
#include <string>
enum class BallState
{
	NORMAL,
	EXPLOSION,
	BALLSTATE_MAX
};
class Ball : public Object
{
	public:
		static ALLEGRO_BITMAP* get_bitmap(BallState type);
		static Ball* ball_create(const Point &p, BallState type);
		void update();
		void draw();

	private:
		double speed = 10.0;
		int attack_freq = 10;
		int counter = 0;
};

#endif // BALL_H_INCLUDED
