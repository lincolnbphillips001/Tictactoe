#ifndef GAME_H
#define GAME_H

#include <goocanvas.h>

class Game {

   public:

	// This is the constructor
	Game ();
	~Game (void);

	// Member functions definitions
	void get_game_info (void);
	void set_current_board (gint **);
	void display_board (void);
	void set_game_status (gboolean);
	gboolean get_game_status (void);
	void reset_number_of_moves (void);
	void increment_number_of_moves (void);
	gint check_board_for_win (void);

	private:
	
	gint **gCurrentTicTacToeBoard;
	gboolean gGameStatus;
	gint gNumberOfMoves;

};

#endif //GAME_H
