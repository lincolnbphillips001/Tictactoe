#ifndef GAME_H
#define GAME_H

#include <gtk/gtk.h>

const gboolean GAME_OVER = 0;
const gboolean GAME_RUNNING = 1;

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

	private:
	
	guint **gCurrentTicTacToeBoard;
	gboolean gGameStatus;
	guint gNumberOfMoves;

};

#endif //GAME_H
