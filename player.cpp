#include <cstdlib>
#include <iostream>
#include <gtk/gtk.h>
#include "player.h"

using namespace std;

const guint NUMBER_OF_BOXES = 9;


Player::Player (guint numberOfPlayers, guint currentPlayer) {

	gNumberOfPlayers = numberOfPlayers;
	set_current_player(currentPlayer);
	gPlayerMoveRecord = new PlayerMoveRecord[NUMBER_OF_BOXES];
	gPlayerMoveRecordIndex = 0;
}

Player::~Player (void) {
 
	if (gPlayerMoveRecord != NULL) {
		delete [] gPlayerMoveRecord;
	}
}

void Player::set_current_player (guint currentPlayer) {
	gCurrentPlayer = currentPlayer;
}

guint Player::get_current_player (void) {
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

void Player::record_player_move (guint numberOfMove, guint currentPlayer, guint boxNumber) {

	gPlayerMoveRecord[numberOfMove].player = currentPlayer;
	gPlayerMoveRecord[numberOfMove].boxSelected = boxNumber;
	gPlayerMoveRecordIndex++;
}

PlayerMoveRecord* Player::get_player_moves (void) {

	return gPlayerMoveRecord;
}

void Player::reset_player_moves (void) {

	if (gPlayerMoveRecordIndex > 0) {

		for (guint i = 0; i < gPlayerMoveRecordIndex; i++) {
			gPlayerMoveRecord[i].player = 0;
			gPlayerMoveRecord[i].boxSelected = 0;
		}
	}
	gPlayerMoveRecordIndex = 0;
}
