#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "ball/Ball.h"
#include "UI.h"

/**
 * @brief Main class that runs the whole game.
 * @details All game procedures must be processed through this class.
 */

typedef struct ALGIF_ANIMATION ALGIF_ANIMATION;
class Game
{
public:
	void execute();
public:
	Game();
	~Game();
	void game_init();
	bool game_update();
	void game_draw();
	void end_game();
private:
	/**
	 * @brief States of the game process in game_update.
	 * @see Game::game_update()
	 */
	enum class STATE {
		START_MENU, // -> LEVEL
		START, // -> START_MENU, LEVEL
		LEVEL, // -> PAUSE, END
		PAUSE, // -> LEVEL
		END
	};
	STATE state;
	ALLEGRO_EVENT event;
	ALLEGRO_BITMAP *game_icon;
	ALLEGRO_BITMAP *background;
	ALLEGRO_BITMAP *start;
	ALGIF_ANIMATION *end;
private:
	ALLEGRO_DISPLAY *display;
	ALLEGRO_TIMER *timer;
	ALLEGRO_EVENT_QUEUE *event_queue;
	UI *ui;
	bool inMenu = true;
};

#endif
