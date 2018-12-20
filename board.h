#ifndef BOARD_H
#define BOARD_H

#include <goocanvas.h>

class Board {
  
	public:

	// This is the constructor
	Board (gint);
	// This is the destructor
	~Board (void);

	// Member functions definitions
	void get_board_info (void);
	void set_box_clicked (gint);
	gboolean get_box_clicked (gint);
	void update_board (gint, gint);
	void display_board (void);
	gboolean is_board_empty(void);
	gboolean is_board_full(void);
	gint** get_current_board (void);
	void reset_board (void);

	private:

	gint gNumOfBoxes;
	gboolean *gBoxClicked;
	gint **gTicTacToeBoard;

};

#endif //BOARD_H