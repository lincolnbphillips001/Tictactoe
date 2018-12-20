#include <iostream>
#include <goocanvas.h>
#include "game.h"

using namespace std;

const gint NUMBER_OF_ROWS = 3;
const gint NUMBER_OF_COLUMNS = 3;
const gint MIN_MOVES_FOR_WIN = 5;
const gint ROW_1 = 0;
const gint ROW_2 = 1;
const gint ROW_3 = 2;
const gint COL_1 = 0;
const gint COL_2 = 1;
const gint COL_3 = 2;
const gint NO_WIN = 0;
const gint PLAYER_1_NUMBER = 1;
const gint PLAYER_2_NUMBER = 2;
const gint ZERO_SUM = 0;

Game::Game (void) {

	gCurrentTicTacToeBoard = new gint*[NUMBER_OF_ROWS];
	for(int i = 0; i < NUMBER_OF_ROWS; ++i) {
    	gCurrentTicTacToeBoard[i] = new int[NUMBER_OF_COLUMNS];
	}
}

Game::~Game (void) {

	for(int i = 0; i < NUMBER_OF_ROWS; ++i) {
    	delete [] gCurrentTicTacToeBoard[i];
	}

	delete [] gCurrentTicTacToeBoard;
}

void Game::set_current_board (gint **currentTicTacToeBoard) {

	for (int i = 0; i < NUMBER_OF_ROWS; i++) {
		for (int j = 0; j < NUMBER_OF_COLUMNS; j++) {
			gCurrentTicTacToeBoard[i][j] = currentTicTacToeBoard[i][j];
		}
	}
}

void Game::display_board (void) {

	for (int i = 0; i < NUMBER_OF_ROWS; i++) {
		for (int j = 0; j < NUMBER_OF_COLUMNS; j++) {
			cout << gCurrentTicTacToeBoard[i][j] << " ";
		}
		cout << endl;
	}
	cout << "-----" << endl;
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

void Game::increment_number_of_moves(void) {
	
	gNumberOfMoves++;
}

gint Game::check_board_for_win (void) {

	if (gNumberOfMoves >= MIN_MOVES_FOR_WIN) {

		gint 	r1_c1, r1_c2, r1_c3, 
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

				if (r1_c3 == PLAYER_1_NUMBER) {
					return PLAYER_1_NUMBER;
				} else {
					return PLAYER_2_NUMBER;
				}
			}
		}
	}

	return NO_WIN;
}
