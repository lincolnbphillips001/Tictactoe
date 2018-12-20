#include <cstdlib>
#include <iostream>
#include <goocanvas.h>
#include "board.h"

using namespace std;

const gint NUMBER_OF_ROWS = 3;
const gint NUMBER_OF_COLUMNS = 3;
const gint NO_PLAYER = 0;

Board::Board (gint numOfBoxes) {

	gNumOfBoxes = numOfBoxes;

	gBoxClicked = new gboolean[numOfBoxes];

	gTicTacToeBoard = new gint*[NUMBER_OF_ROWS];
	for(int i = 0; i < NUMBER_OF_ROWS; ++i) {
    	gTicTacToeBoard[i] = new int[NUMBER_OF_COLUMNS];
	}

	//initialize all false
	for (int i = 0; i < numOfBoxes; i++) {
		gBoxClicked[i] = FALSE;
	}

	for (int i = 0; i < NUMBER_OF_ROWS; i++) {
		for (int j = 0; j < NUMBER_OF_COLUMNS; j++) {
			gTicTacToeBoard[i][j] = FALSE;
		}
	}
}

Board::~Board (void) {
   
    delete [] gBoxClicked;

	for(int i = 0; i < NUMBER_OF_ROWS; ++i) {
    	delete [] gTicTacToeBoard[i];
	}

	delete [] gTicTacToeBoard;
}

void Board::set_box_clicked (gint boxNumber) {
	gBoxClicked[boxNumber] = TRUE;
}

gboolean Board::get_box_clicked (gint boxNumber) {
	return gBoxClicked[boxNumber];
}

void Board::update_board (gint boxNumber, gint player) {
	
	gint rowIndex;
	gint columnIndex;

	rowIndex = (boxNumber-1) / NUMBER_OF_ROWS;
	columnIndex = (boxNumber-1) % NUMBER_OF_COLUMNS;

	//turn 0 into 1 and 1 into 2
	gTicTacToeBoard[rowIndex][columnIndex] = player+1;
}

void Board::display_board (void) {

	for (int i = 0; i < NUMBER_OF_ROWS; i++) {
		for (int j = 0; j < NUMBER_OF_COLUMNS; j++) {
			cout << gTicTacToeBoard[i][j] << " ";
		}
		cout << endl;
	}
}

gboolean Board::is_board_empty (void) {

	gboolean isBoardEmpty = TRUE;

	for (int i = 0; i < NUMBER_OF_ROWS; i++) {
		for (int j = 0; j < NUMBER_OF_COLUMNS; j++) {
			if (gTicTacToeBoard[i][j] == FALSE) {
				isBoardEmpty = TRUE;
			} else {
				isBoardEmpty = FALSE;
				break;
			}
		}
		if (isBoardEmpty == FALSE) {
			break;
		}
	}

	return isBoardEmpty;
}

gboolean Board::is_board_full (void) {

	gboolean isBoardFull = TRUE;

	for (int i = 0; i < NUMBER_OF_ROWS; i++) {
		for (int j = 0; j < NUMBER_OF_COLUMNS; j++) {
			if (gTicTacToeBoard[i][j] == FALSE) {
				isBoardFull = FALSE;
				break;
			} else {
				isBoardFull = TRUE;
			}
		}
		if (isBoardFull == FALSE) {
			break;
		}
	}

	return isBoardFull;
}

gint** Board::get_current_board (void) {

	return gTicTacToeBoard;
}

void Board::reset_board (void) {

	//initialize all false
	for (int i = 0; i < gNumOfBoxes; i++) {
		gBoxClicked[i] = FALSE;
	}

	for (int i = 0; i < NUMBER_OF_ROWS; i++) {
		for (int j = 0; j < NUMBER_OF_COLUMNS; j++) {
			gTicTacToeBoard[i][j] = NO_PLAYER;
		}
	}
}

