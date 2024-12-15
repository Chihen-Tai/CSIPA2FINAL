#include "Game.h"
#include "Utils.h"
#include "data/DataCenter.h"
#include "data/OperationCenter.h"
#include "data/SoundCenter.h"
#include "data/ImageCenter.h"
#include "data/FontCenter.h"
#include "Player.h"
#include "Level.h"

#include "Hero/Hero.h"

#include "block/block.h"

#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_acodec.h>
#include <vector>
#include <cstring>

// fixed settings
constexpr char main_menu_img_path[] = "./assets/image/menu_game.png";
constexpr char game_icon_img_path[] = "./assets/image/game_icon.png";
constexpr char game_start_sound_path[] = "./assets/sound/start.mp3";
constexpr char background_img_path[] = "./assets/image/StartBackground.png";
constexpr char background_sound_path[] = "./assets/sound/bossfight.mp3";
constexpr char end_background_sound_path[] = "./assets/sound/ymniam.wav";

/**
 * @brief Game entry.
 * @details The function processes all allegro events and update the event state to a generic data storage (i.e. DataCenter).
 * For timer event, the game_update and game_draw function will be called if and only if the current is timer.
 */
void Game::execute()
{
	DataCenter *DC = DataCenter::get_instance();
	// main game loop
	bool run = true;
	while (run)
	{
		// process all events here
		al_wait_for_event(event_queue, &event);
		switch (event.type)
		{
		case ALLEGRO_EVENT_TIMER:
		{
			run &= game_update();
			game_draw();
			break;
		}
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
		{ // stop game
			run = false;
			break;
		}
		case ALLEGRO_EVENT_KEY_DOWN:
		{
			DC->key_state[event.keyboard.keycode] = true;
			break;
		}
		case ALLEGRO_EVENT_KEY_UP:
		{
			DC->key_state[event.keyboard.keycode] = false;
			break;
		}
		case ALLEGRO_EVENT_MOUSE_AXES:
		{
			DC->mouse.x = event.mouse.x;
			DC->mouse.y = event.mouse.y;
			break;
		}
		case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
		{
			DC->mouse_state[event.mouse.button] = true;
			break;
		}
		case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
		{
			DC->mouse_state[event.mouse.button] = false;
			break;
		}
		default:
			break;
		}
	}
}

/**
 * @brief Initialize all allegro addons and the game body.
 * @details Only one timer is created since a game and all its data should be processed synchronously.
 */
Game::Game()
{
	DataCenter *DC = DataCenter::get_instance();
	GAME_ASSERT(al_init(), "failed to initialize allegro.");

	// initialize allegro addons
	bool addon_init = true;
	addon_init &= al_init_primitives_addon();
	addon_init &= al_init_font_addon();
	addon_init &= al_init_ttf_addon();
	addon_init &= al_init_image_addon();
	addon_init &= al_init_acodec_addon();
	GAME_ASSERT(addon_init, "failed to initialize allegro addons.");

	// initialize events
	bool event_init = true;
	event_init &= al_install_keyboard();
	event_init &= al_install_mouse();
	event_init &= al_install_audio();
	GAME_ASSERT(event_init, "failed to initialize allegro events.");

	// initialize game body
	GAME_ASSERT(
		display = al_create_display(DC->window_width, DC->window_height),
		"failed to create display.");
	GAME_ASSERT(
		timer = al_create_timer(1.0 / DC->FPS),
		"failed to create timer.");
	GAME_ASSERT(
		event_queue = al_create_event_queue(),
		"failed to create event queue.");

	debug_log("Game initialized.\n");
	game_init();
}

/**
 * @brief Initialize all auxiliary resources.
 */
void Game::game_init()
{
	SoundCenter *SC = SoundCenter::get_instance();
	ImageCenter *IC = ImageCenter::get_instance();
	FontCenter *FC = FontCenter::get_instance();
	// set window icon
	game_icon = IC->get(game_icon_img_path);
	al_set_display_icon(display, game_icon);

	// register events to event_queue
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	// init sound setting
	SC->init();

	// init font setting
	FC->init();

	// game start
	background = IC->get(background_img_path);
	start = IC->get(main_menu_img_path);
	debug_log("Game state: change to START\n");
	state = STATE::START_MENU;
	al_start_timer(timer);
}

/**
 * @brief The function processes all data update.
 * @details The behavior of the whole game body is determined by its state.
 * @return Whether the game should keep running (true) or reaches the termination criteria (false).
 * @see Game::STATE
 */
bool Game::game_update()
{
	DataCenter *DC = DataCenter::get_instance();
	OperationCenter *OC = OperationCenter::get_instance();
	SoundCenter *SC = SoundCenter::get_instance();
	static ALLEGRO_SAMPLE_INSTANCE *background = nullptr;
	static bool BGM_played = false;
	switch (state)
	{
	case STATE::START_MENU:
	{
		static bool is_played = false;
		static ALLEGRO_SAMPLE_INSTANCE *instance = nullptr;
		if (!is_played)
		{
			for(int i=0;i<5;i++)
			{
				instance = SC->play(game_start_sound_path, ALLEGRO_PLAYMODE_ONCE);
			}
			DC->level->init();
			DC->level->load_level(1);
			ui = new UI();
			ui->init(this);
			DC->hero->init();
			is_played = true;
		}

		if (!SC->is_playing(instance))
		{
			debug_log("<Game> state: change to LEVEL\n");
			state = STATE::LEVEL;
		}
		break;
	}
	case STATE::START:
	{
		static bool is_played = false;
		static ALLEGRO_SAMPLE_INSTANCE *instance = nullptr;
		if (!is_played)
		{
			instance = SC->play(game_start_sound_path, ALLEGRO_PLAYMODE_ONCE);
			DC->level->load_level(1);
			is_played = true;
		}

		if (!SC->is_playing(instance))
		{
			debug_log("<Game> state: change to LEVEL\n");
			state = STATE::LEVEL;
		}
		break;
	}
	case STATE::LEVEL:
	{
		if (inMenu) {
			if (DC->key_state[ALLEGRO_KEY_ENTER] && !DC->prev_key_state[ALLEGRO_KEY_ENTER])
			{
				inMenu = false;
			}

			if (DC->key_state[ALLEGRO_KEY_ESCAPE] && !DC->prev_key_state[ALLEGRO_KEY_ESCAPE])
			{
				debug_log("<Game> state: change to END\n");
				return false;
			}
			break;
		}

		

		if (!BGM_played)
		{
			
			background = SC->play(background_sound_path, ALLEGRO_PLAYMODE_LOOP);
			BGM_played = true;
		}

		if (DC->key_state[ALLEGRO_KEY_P] && !DC->prev_key_state[ALLEGRO_KEY_P])
		{
			SC->toggle_playing(background);
			debug_log("<Game> state: change to PAUSE\n");
			state = STATE::PAUSE;
		}
		break;
	}
	case STATE::PAUSE:
	{
		if (DC->key_state[ALLEGRO_KEY_P] && !DC->prev_key_state[ALLEGRO_KEY_P])
		{
			SC->toggle_playing(background);
			debug_log("<Game> state: change to LEVEL\n");
			state = STATE::LEVEL;
		}
		break;
	}
	case STATE::END:
	{
		if(!end_BGM_played){
			end_background = SC->play(end_background_sound_path, ALLEGRO_PLAYMODE_LOOP);
			end_BGM_played = true;
		}
		if(DC->key_state[ALLEGRO_KEY_ENTER] && !DC->prev_key_state[ALLEGRO_KEY_ENTER])
		{
			// delete ui;
			// ui = new UI();
			// ui->init(this);
			// DC->level->init();
			// DC->player->coin = 100000000;
			// DC->hero->init();
			// for(auto &block:DC->blocks)
			// {
			// 	delete block;
			// }
			// DC->blocks.clear();
			// SC->toggle_playing(background);
			// inMenu = true;
			// debug_log("<Game> state: change to START\n");
			//state = STATE::START;
			return false;
		}
	}
	}
	// If the game is not paused, we should progress update.
	if (state == STATE::LEVEL && !inMenu)
	{
		DC->player->update();
		SC->update();
		ui->update();
		DC->hero->update();
		DC->level->update();
		if (state != STATE::START && !inMenu)
		{
			OC->update();
		}
	}

	// game_update is finished. The states of current frame will be previous states of the next frame.
	memcpy(DC->prev_key_state, DC->key_state, sizeof(DC->key_state));
	memcpy(DC->prev_mouse_state, DC->mouse_state, sizeof(DC->mouse_state));
	return true;
}

/**
 * @brief Draw the whole game and objects.
 */
void Game::game_draw()
{
	DataCenter *DC = DataCenter::get_instance();
	OperationCenter *OC = OperationCenter::get_instance();
	FontCenter *FC = FontCenter::get_instance();

	// Flush the screen first.
	al_clear_to_color(al_map_rgb(100, 100, 100));
	if (state == STATE::LEVEL && inMenu)
	{
		al_draw_bitmap(start, 0, 0, 0);
		al_draw_text(
			FC->caviar_dreams[FontSize::LARGE], al_map_rgb(0, 0, 0),
			DC->window_width / 2., DC->window_height / 2.,
			ALLEGRO_ALIGN_CENTRE, "Press enter to start");
		al_draw_text(
			FC->caviar_dreams[FontSize::MEDIUM], al_map_rgb(0, 0, 0),
			DC->window_width / 2., DC->window_height / 2. + 50,
			ALLEGRO_ALIGN_CENTRE, "Press P to pause in game");
		al_draw_text(
			FC->caviar_dreams[FontSize::MEDIUM], al_map_rgb(0, 0, 0),
			DC->window_width / 2., DC->window_height / 2. + 100,
			ALLEGRO_ALIGN_CENTRE, "Press esc to exit");
	}
	if (state == STATE::LEVEL)
	{
		// background
		if (!inMenu)
			al_draw_bitmap(background, 0, 0, 0);
		// user interface
		if (state != STATE::START && !inMenu)
		{
			OC->draw();
			DC->hero->draw();
			ui->draw();
		}
	}
	switch (state)
	{
	case STATE::START:
	{
		break;
	}
	case STATE::START_MENU:
	{
		break;
	}
	case STATE::LEVEL:
	{
		break;
	}
	case STATE::PAUSE:
	{
		// game layout cover
		al_draw_filled_rectangle(0, 0, DC->window_width, DC->window_height, al_map_rgba(50, 50, 50, 64));
		al_draw_text(
			FC->caviar_dreams[FontSize::LARGE], al_map_rgb(255, 255, 255),
			DC->window_width / 2., DC->window_height / 2.,
			ALLEGRO_ALIGN_CENTRE, "GAME PAUSED");
		break;
	}
	case STATE::END:
	{
		if (!inMenu)
			al_draw_bitmap(background, 0, 0, 0);
		al_draw_text(
			FC->caviar_dreams[FontSize::LARGE], al_map_rgb(0, 0, 0),
			DC->window_width / 2., DC->window_height / 2.,
			ALLEGRO_ALIGN_CENTRE, "YOU WIN!");
		al_draw_textf(
			FC->caviar_dreams[FontSize::LARGE], al_map_rgb(0, 0, 0),
			DC->window_width / 2., DC->window_height / 2. + 50,
			ALLEGRO_ALIGN_CENTRE, "Score: %d", DC->player->coin);
		al_draw_text(FC->caviar_dreams[FontSize::MEDIUM], al_map_rgb(0, 0, 0),
					 DC->window_width / 2., DC->window_height / 2. +100,
					 ALLEGRO_ALIGN_CENTRE, "Press enter to exit");
	}
	}
	al_flip_display();
}

Game::~Game()
{
	al_destroy_display(display);
	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);
}

void Game::end_game()
{
	state = STATE::END;
}