#ifndef GAME_H
#define GAME_H

#include <gtk/gtk.h>

const gboolean GAME_OVER = 0;
const gboolean GAME_RUNNING = 1;

const guint NO_CURRENT_WIN = 0;
const guint TOP_ROW = 1;
const guint MIDDLE_ROW = 2;
const guint BOTTOM_ROW = 3;
const guint LEFT_COLUMN = 4;
const guint MIDDLE_COLUMN = 5;
const guint RIGHT_COLUMN = 6;
const guint TOP_LEFT_TO_BOTTOM_RIGHT = 7;
const guint TOP_RIGHT_TO_BOTTOM_LEFT = 8;

const guint NO_WIN = 0;
const guint PLAYER_1_NUMBER = 1;
const guint PLAYER_2_NUMBER = 2;

class Game {

   public:

	// This is the constructor
	Game ();
	~Game (void);

	// Member functions definitions
	void set_current_board (guint **);
	void reset_current_board ();
	void display_board (void);
	void set_game_status (gboolean);
	gboolean get_game_status (void);
	void reset_number_of_moves (void);
	guint get_number_of_moves (void);
	void increment_number_of_moves (void);
	guint check_board_for_win (void);
	guint get_win_direction (void);
	void reset_win_direction (void);

	private:
	
	guint **gCurrentTicTacToeBoard;
	gboolean gGameStatus;
	guint gNumberOfMoves;
	guint gDirectionOfWin;

};

#endif //GAME_H
