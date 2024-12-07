#ifndef UI_H_INCLUDED
#define UI_H_INCLUDED

#include <allegro5/bitmap.h>
#include <vector>
#include <tuple>
#include "./shapes/Point.h"
class Game;
class UI
{
public:
	UI() {}
	void init(Game* game);
	void update();
	void draw();
private:
	enum class STATE {
		HALT, // -> HOVER
		HOVER // -> HALT, SELECT
		// SELECT, // -> HALT, PLACE
		// PLACE // -> HALT
	};
	STATE state;
	ALLEGRO_BITMAP *love;
	// tower menu bitmap, (top-left x, top-left y), price
	std::vector<std::tuple<ALLEGRO_BITMAP*, Point, int>> tower_items;
	int on_item;
	Game *game;
};

#endif
