#include "UI.h"
#include "Utils.h"
#include "data/DataCenter.h"
#include "data/ImageCenter.h"
#include "data/FontCenter.h"
#include "data/SoundCenter.h"
#include <algorithm>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include "shapes/Point.h"
#include "shapes/Rectangle.h"
#include "Player.h"
#include "towers/Tower.h"
#include "ball/ball.h"
#include "Level.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include "Game.h"
// fixed settings
constexpr char love_img_path[] = "./assets/image/love.png";
constexpr char success_sound_path[] = "./assets/sound/success.mp3";
constexpr char fail_sound_path[] = "./assets/sound/fail.mp3";
constexpr int love_img_padding = 5;
constexpr int tower_img_left_padding = 30;
constexpr int tower_img_top_padding = 30;
int ball_count = 0;
void UI::init(Game *game)
{
	DataCenter *DC = DataCenter::get_instance();
	ImageCenter *IC = ImageCenter::get_instance();
	this->game = game;
	love = IC->get(love_img_path);
	int tl_x = DC->game_field_length + tower_img_left_padding;
	int tl_y = tower_img_top_padding;
	int max_height = 0;
	// arrange tower shop
	for (size_t i = 0; i < (size_t)(TowerType::TOWERTYPE_MAX); ++i)
	{
		ALLEGRO_BITMAP *bitmap = IC->get(TowerSetting::tower_menu_img_path[i]);
		int w = al_get_bitmap_width(bitmap);
		int h = al_get_bitmap_height(bitmap);
		if (tl_x + w > DC->window_width)
		{
			tl_x = DC->game_field_length + tower_img_left_padding;
			tl_y += max_height + tower_img_top_padding;
			max_height = 0;
		}
		tower_items.emplace_back(bitmap, Point{tl_x, tl_y}, TowerSetting::tower_price[i]);
		tl_x += w + tower_img_left_padding;
		max_height = std::max(max_height, h);
	}
	debug_log("<UI> state: change to HALT\n");
	state = STATE::HALT;
	on_item = -1;
}

#include <allegro5/allegro_video.h>

void UI::update()
{
	DataCenter *DC = DataCenter::get_instance();
	const Point &mouse = DC->mouse;
	SoundCenter *SC = SoundCenter::get_instance();
	switch (state)
	{
	case STATE::HALT:
	{
		for (size_t i = 0; i < tower_items.size(); ++i)
		{
			auto &[bitmap, p, price] = tower_items[i];
			int w = al_get_bitmap_width(bitmap);
			int h = al_get_bitmap_height(bitmap);
			// hover on a shop tower item
			if (mouse.overlap(Rectangle{p.x, p.y, p.x + w, p.y + h}))
			{
				on_item = i;
				debug_log("<UI> state: change to HOVER\n");
				state = STATE::HOVER;
				break;
			}
		}
		break;
	}
	case STATE::HOVER:
	{
		auto &[bitmap, p, price] = tower_items[on_item];
		int w = al_get_bitmap_width(bitmap);
		int h = al_get_bitmap_height(bitmap);
		if (!mouse.overlap(Rectangle{p.x, p.y, p.x + w, p.y + h}))
		{
			on_item = -1;
			debug_log("<UI> state: change to HALT\n");
			state = STATE::HALT;
			break;
		}
		// click mouse left button
		if (DC->mouse_state[1] && !DC->prev_mouse_state[1])
		{
			// add coin
			if (on_item == 0)
			{
				if (DC->player->coin < 50)
				{
					SC->play(fail_sound_path, ALLEGRO_PLAYMODE_ONCE);
					break;
				}
				SC->play(success_sound_path, ALLEGRO_PLAYMODE_ONCE);
				DC->player->coin -= 50;
				DC->balls.emplace_back(Ball::create_ball(BallState::Normal));
				ball_count++;
			}
			if (on_item == 1)
			{
				if (DC->player->coin < 100)
				{
					SC->play(fail_sound_path, ALLEGRO_PLAYMODE_ONCE);
					break;
				}
				SC->play(success_sound_path, ALLEGRO_PLAYMODE_ONCE);
				DC->player->coin -= 100;
				Ball::speed *= 1.1;
				for (auto &ball : DC->balls)
				{
					ball->set_vx(ball->get_vx() * 1.1);
					ball->set_vy(ball->get_vy() * 1.1);
				}
			}
			if (on_item == 2)
			{
				if (DC->player->coin < 150)
				{
					SC->play(fail_sound_path, ALLEGRO_PLAYMODE_ONCE);
					break;
				}
				SC->play(success_sound_path, ALLEGRO_PLAYMODE_ONCE);
				DC->player->coin -= 150;
				Ball::buy_damage += 1;
				for (auto &ball : DC->balls)
				{
					ball->setDamage(ball->getDamage() + 1);
				}
			}
			if (on_item == 3)
			{
				if (DC->player->coin < 200)
				{
					SC->play(fail_sound_path, ALLEGRO_PLAYMODE_ONCE);
					break;
				}
				SC->play(success_sound_path, ALLEGRO_PLAYMODE_ONCE);
				DC->player->coin -= 200;
				DC->balls.emplace_back(Ball::create_ball(BallState::BIG));
				ball_count++;
			}
			if (on_item == 4)
			{
				if (DC->player->coin < 1000000)
				{
					SC->play(fail_sound_path, ALLEGRO_PLAYMODE_ONCE);
					break;
				}
				SC->play(success_sound_path, ALLEGRO_PLAYMODE_ONCE);
				// DC->player->coin -= 1000000;
				Ball::speed = 1;
				Ball::buy_damage = 1;
				for (auto &ball : DC->balls)
				{
					delete ball;
				}
				DC->balls.clear();
				ball_count = 0;
				//SC->toggle_playing(DC->background);
				this->game->end_game();
			}
		}
		break;
	}
	}
}

void UI::draw()
{
	DataCenter *DC = DataCenter::get_instance();
	FontCenter *FC = FontCenter::get_instance();
	// draw HP
	const int &game_field_length = DC->game_field_length;
	const int &player_HP = DC->player->HP;
	int love_width = al_get_bitmap_width(love);
	for (int i = 1; i <= player_HP; ++i)
	{
		al_draw_bitmap(love, game_field_length - (love_width + love_img_padding) * i, love_img_padding, 0);
	}
	// draw coin
	const int &player_coin = DC->player->coin;
	al_draw_textf(
		FC->courier_new[FontSize::MEDIUM], al_map_rgb(0, 0, 0),
		game_field_length + love_img_padding, love_img_padding,
		ALLEGRO_ALIGN_LEFT, "scores: %5d", player_coin);
	al_draw_textf(
		FC->courier_new[FontSize::MEDIUM], al_map_rgb(0, 0, 0),
		0, love_img_padding * 5,
		ALLEGRO_ALIGN_LEFT, "speed: %.4lf", Ball::speed);
	al_draw_textf(
		FC->courier_new[FontSize::MEDIUM], al_map_rgb(0, 0, 0),
		0, love_img_padding * 10,
		ALLEGRO_ALIGN_LEFT, "damage: %3d", Ball::buy_damage);
	al_draw_textf(
		FC->courier_new[FontSize::MEDIUM], al_map_rgb(0, 0, 0),
		0, love_img_padding * 15,
		ALLEGRO_ALIGN_LEFT, "ball count: %3d", ball_count);
	al_draw_textf(
		FC->courier_new[FontSize::MEDIUM], al_map_rgb(0, 0, 0),
		0, love_img_padding,
		ALLEGRO_ALIGN_LEFT, "level: %3d", DC->level->get_level());
	// draw tower shop items
	for (auto &[bitmap, p, price] : tower_items)
	{
		int w = al_get_bitmap_width(bitmap);
		int h = al_get_bitmap_height(bitmap);
		al_draw_bitmap(bitmap, p.x, p.y, 0);
		al_draw_rectangle(
			p.x - 1, p.y - 1,
			p.x + w + 1, p.y + h + 1,
			al_map_rgb(0, 0, 0), 1);
		al_draw_textf(
			FC->courier_new[FontSize::MEDIUM], al_map_rgb(0, 0, 0),
			p.x + w / 2, p.y + h,
			ALLEGRO_ALIGN_CENTRE, "%d", price);
	}

	switch (state)
	{
		static Tower *selected_tower = nullptr;
	case STATE::HALT:
	{
		// No tower should be selected for HALT state.
		if (selected_tower != nullptr)
		{
			delete selected_tower;
			selected_tower = nullptr;
		}
		break;
	}
	case STATE::HOVER:
	{
		auto &[bitmap, p, price] = tower_items[on_item];
		int w = al_get_bitmap_width(bitmap);
		int h = al_get_bitmap_height(bitmap);
		// Create a semitransparent mask covered on the hovered tower.
		al_draw_filled_rectangle(p.x, p.y, p.x + w, p.y + h, al_map_rgba(50, 50, 50, 64));
		break;
	}
	}
}
