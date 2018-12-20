#include <cstdlib>
#include <iostream>
#include <goocanvas.h>
#include "player.h"

using namespace std;

const gint PLAYER_1 = 0;
const gint PLAYER_2 = 1;


Player::Player (gint numberOfPlayers, gint currentPlayer) {

	gNumberOfPlayers = numberOfPlayers;
	set_current_player(currentPlayer);
}

Player::~Player (void) {
   
     //Nothing to do
}

void Player::set_current_player (gint currentPlayer) {
	gCurrentPlayer = currentPlayer;
}

gint Player::get_current_player (void) {
	return gCurrentPlayer;
}

void Player::switch_player (void) {

	switch (get_current_player()) {
		case PLAYER_1:
			gCurrentPlayer = PLAYER_2;
			break;
		case PLAYER_2:
			gCurrentPlayer = PLAYER_1;
			break;
	}
}