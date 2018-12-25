#include <cstdlib>
#include <cstring>
#include <iostream>
#include <gtk/gtk.h>
#include "game.h"

using namespace std;

const guint NUMBER_OF_ROWS = 3;
const guint NUMBER_OF_COLUMNS = 3;
const guint MIN_MOVES_FOR_WIN = 5;
const guint ROW_1 = 0;
const guint ROW_2 = 1;
const guint ROW_3 = 2;
const guint COL_1 = 0;
const guint COL_2 = 1;
const guint COL_3 = 2;

const guint ZERO_SUM = 0;

Game::Game (void) {

	gCurrentTicTacToeBoard = new guint*[NUMBER_OF_ROWS];
	for(guint i = 0; i < NUMBER_OF_ROWS; i++) {
    	gCurrentTicTacToeBoard[i] = new guint[NUMBER_OF_COLUMNS];
	}
	gNumberOfMoves = 0;

	for (guint i = 0; i < NUMBER_OF_ROWS; i++) {
    	memset(&gCurrentTicTacToeBoard[i], 0, sizeof(gCurrentTicTacToeBoard[0]));
	}

	gDirectionOfWin = NO_CURRENT_WIN;
}

Game::~Game (void) {

	for(guint i = 0; i < NUMBER_OF_ROWS; i++) {
    	delete [] gCurrentTicTacToeBoard[i];
	}

	delete [] gCurrentTicTacToeBoard;
}

void Game::set_current_board (guint **currentTicTacToeBoard) {

	for (guint i = 0; i < NUMBER_OF_ROWS; i++) {
    	memcpy(&gCurrentTicTacToeBoard[i], 
    		&currentTicTacToeBoard[i], sizeof(currentTicTacToeBoard[0]));
	}
}

void Game::reset_current_board (void) {

	for (guint i = 0; i < NUMBER_OF_ROWS; i++) {
    	memset(&gCurrentTicTacToeBoard[i], 0, sizeof(gCurrentTicTacToeBoard[0]));
	}
}

void Game::display_board (void) {

	for (guint i = 0; i < NUMBER_OF_ROWS; i++) {
		for (guint j = 0; j < NUMBER_OF_COLUMNS; j++) {
			cout << gCurrentTicTacToeBoard[i][j] << " ";
		}
		cout << endl;
	}
}

void Game::set_game_status (gboolean gameStatus) {

	gGameStatus = gameStatus;
}

gboolean Game::get_game_status (void) {

	return gGameStatus;
}

void Game::reset_number_of_moves(void) {
	
	gNumberOfMoves = 0;
}

guint Game::get_number_of_moves(void) {
	
	return gNumberOfMoves;
}

void Game::increment_number_of_moves(void) {
	
	gNumberOfMoves++;
}

guint Game::check_board_for_win (void) {

	if (gNumberOfMoves >= MIN_MOVES_FOR_WIN) {

		guint 	r1_c1, r1_c2, r1_c3, 
				r2_c1, r2_c2, r2_c3, 
				r3_c1, r3_c2, r3_c3;

		r1_c1 = gCurrentTicTacToeBoard[ROW_1][COL_1];
		r1_c2 = gCurrentTicTacToeBoard[ROW_1][COL_2];
		r1_c3 = gCurrentTicTacToeBoard[ROW_1][COL_3];
		r2_c1 = gCurrentTicTacToeBoard[ROW_2][COL_1];
		r2_c2 = gCurrentTicTacToeBoard[ROW_2][COL_2];
		r2_c3 = gCurrentTicTacToeBoard[ROW_2][COL_3];
		r3_c1 = gCurrentTicTacToeBoard[ROW_3][COL_1];
		r3_c2 = gCurrentTicTacToeBoard[ROW_3][COL_2];
		r3_c3 = gCurrentTicTacToeBoard[ROW_3][COL_3];

		//check top row
		if ((r1_c1 + r1_c2 + r1_c3) != ZERO_SUM) { 
			
			if ((r1_c1 == r1_c2) && (r1_c1 == r1_c3) && (r1_c2 == r1_c3)) {

				gDirectionOfWin = TOP_ROW;

				if (r1_c1 == PLAYER_1_NUMBER) {
					return PLAYER_1_NUMBER;
				} else {
					return PLAYER_2_NUMBER;
				}
			}
		}

		//check middle row
		if ((r2_c1 + r2_c2 + r2_c3) != ZERO_SUM) { 
			
			if ((r2_c1 == r2_c2) && (r2_c1 == r2_c3) && (r2_c2 == r2_c3)) {

				gDirectionOfWin = MIDDLE_ROW;

				if (r2_c1 == PLAYER_1_NUMBER) {
					return PLAYER_1_NUMBER;
				} else {
					return PLAYER_2_NUMBER;
				}
			}
		}

		//check bottom row
		if ((r3_c1 + r3_c2 + r3_c3) != ZERO_SUM) { 
			
			if ((r3_c1 == r3_c2) && (r3_c1 == r3_c3) && (r3_c2 == r3_c3)) {

				gDirectionOfWin = BOTTOM_ROW;

				if (r3_c1 == PLAYER_1_NUMBER) {
					return PLAYER_1_NUMBER;
				} else {
					return PLAYER_2_NUMBER;
				}
			}
		}

		//check left column
		if ((r1_c1 + r2_c1 + r3_c1) != ZERO_SUM) { 
			
			if ((r1_c1 == r2_c1) && (r1_c1 == r3_c1) && (r2_c1 == r3_c1)) {

				gDirectionOfWin = LEFT_COLUMN;

				if (r1_c1 == PLAYER_1_NUMBER) {
					return PLAYER_1_NUMBER;
				} else {
					return PLAYER_2_NUMBER;
				}
			}
		}
		//check middle column
		if ((r1_c2 + r2_c2 + r3_c2) != ZERO_SUM) { 
			
			if ((r1_c2 == r2_c2) && (r1_c2 == r3_c2) && (r2_c2 == r3_c2)) {

				gDirectionOfWin = MIDDLE_COLUMN;

				if (r1_c2 == PLAYER_1_NUMBER) {
					return PLAYER_1_NUMBER;
				} else {
					return PLAYER_2_NUMBER;
				}
			}
		}
		//check right column
		if ((r1_c3 + r2_c3 + r3_c3) != ZERO_SUM) { 
			
			if ((r1_c3 == r2_c3) && (r1_c3 == r3_c3) && (r2_c3 == r3_c3)) {

				gDirectionOfWin = RIGHT_COLUMN;

				if (r1_c3 == PLAYER_1_NUMBER) {
					return PLAYER_1_NUMBER;
				} else {
					return PLAYER_2_NUMBER;
				}
			}
		}

		//check top left and bottom right diagonal
		if ((r1_c1 + r2_c2 + r3_c3) != ZERO_SUM) {

			if ((r1_c1 == r2_c2) && (r1_c1 == r3_c3) && (r2_c2 == r3_c3)) {

				gDirectionOfWin = TOP_LEFT_TO_BOTTOM_RIGHT;

				if (r1_c1 == PLAYER_1_NUMBER) {
					return PLAYER_1_NUMBER;
				} else {
					return PLAYER_2_NUMBER;
				}
			}
		}

		//check top right and bottom left diagonal
		if ((r1_c3 + r2_c2 + r3_c1) != ZERO_SUM) { 

			if ((r1_c3 == r2_c2) && (r1_c3 == r3_c1) && (r2_c2 == r3_c1)) {

				gDirectionOfWin = TOP_RIGHT_TO_BOTTOM_LEFT;

				if (r1_c3 == PLAYER_1_NUMBER) {
					return PLAYER_1_NUMBER;
				} else {
					return PLAYER_2_NUMBER;
				}
			}
		}
	}

	gDirectionOfWin = NO_CURRENT_WIN;
	return NO_WIN;
}

guint Game::get_win_direction (void) {

	return gDirectionOfWin;
}

void Game::reset_win_direction (void) {

	gDirectionOfWin = NO_CURRENT_WIN;
}
