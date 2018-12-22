#include <cstdlib>
#include <iostream>
#include <gtk/gtk.h>
#include "board.h"

using namespace std;

const guint NO_PLAYER = 0;

Board::Board (guint numOfBoxes) {

	gNumOfBoxes = numOfBoxes;

	gBoxClicked = new gboolean[numOfBoxes+1];

	gTicTacToeBoard = new guint*[NUMBER_OF_ROWS];
	for(guint i = 0; i < NUMBER_OF_ROWS; i++) {
    	gTicTacToeBoard[i] = new guint[NUMBER_OF_COLUMNS];
	}

	//initialize all false
	for (guint i = 0; i < (numOfBoxes+1); i++) {
		gBoxClicked[i] = FALSE;
	}

	for (guint i = 0; i < NUMBER_OF_ROWS; i++) {
		for (guint j = 0; j < NUMBER_OF_COLUMNS; j++) {
			gTicTacToeBoard[i][j] = 0;
		}
	}
}

Board::~Board (void) {
  
    delete [] gBoxClicked;

	for(guint i = 0; i < NUMBER_OF_ROWS; i++) {
    	delete [] gTicTacToeBoard[i];
	}

	delete [] gTicTacToeBoard;

}

void Board::set_box_clicked (guint boxNumber) {
	gBoxClicked[boxNumber] = TRUE;
}

gboolean Board::get_box_clicked (guint boxNumber) {
	return gBoxClicked[boxNumber];
}

void Board::update_board (guint boxNumber, guint player) {
	
	guint rowIndex;
	guint columnIndex;

	rowIndex = (boxNumber-1) / NUMBER_OF_ROWS;
	if (rowIndex > 2) {
		rowIndex = 2;
	}
	columnIndex = (boxNumber-1) % NUMBER_OF_COLUMNS;
	if (columnIndex > 2) {
		columnIndex = 2;
	}

	//turn 0 guinto 1 and 1 guinto 2
	gTicTacToeBoard[rowIndex][columnIndex] = player+1;
}

void Board::display_board (void) {

	for (guint i = 0; i < NUMBER_OF_ROWS; i++) {
		for (guint j = 0; j < NUMBER_OF_COLUMNS; j++) {
			cout << gTicTacToeBoard[i][j] << " ";
		}
		cout << endl;
	}
}

gboolean Board::is_board_empty (void) {

	gboolean isBoardEmpty = TRUE;

	for (guint i = 0; i < NUMBER_OF_ROWS; i++) {
		for (guint j = 0; j < NUMBER_OF_COLUMNS; j++) {
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

	for (guint i = 0; i < NUMBER_OF_ROWS; i++) {
		for (guint j = 0; j < NUMBER_OF_COLUMNS; j++) {
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

guint** Board::get_current_board (void) {

	if (gTicTacToeBoard != NULL) {
		return gTicTacToeBoard;
	} else {
		exit (0);
	}

}

void Board::reset_board (void) {

	//initialize all false
	for (guint i = 0; i < (gNumOfBoxes+1); i++) {
		gBoxClicked[i] = FALSE;
	}

	for (guint i = 0; i < NUMBER_OF_ROWS; i++) {
		for (guint j = 0; j < NUMBER_OF_COLUMNS; j++) {
			gTicTacToeBoard[i][j] = 0;
		}
	}


}
