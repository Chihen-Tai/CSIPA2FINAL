#include "Player.h"

// fixed settings
namespace PlayerSetting {
	constexpr int init_HP = -1;
	constexpr int init_coin = 100000;
	constexpr int coin_freq = 0;
	constexpr int coin_increase = 0;
};

Player::Player() : HP(PlayerSetting::init_HP), coin(PlayerSetting::init_coin) {
	this->coin_freq = PlayerSetting::coin_freq;
	this->coin_increase = PlayerSetting::coin_increase;
	coin_counter = PlayerSetting::coin_freq;
}

void
Player::update() {
	if(coin_counter) --coin_counter;
	else {
		coin += coin_increase;
		coin_counter = coin_freq;
	}
}
