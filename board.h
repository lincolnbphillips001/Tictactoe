#ifndef BOARD_H
#define BOARD_H

#include <gtk/gtk.h>

const guint BOARD_NOT_FULL = 0;
const guint BOARD_IS_FULL = 1;
const guint NUMBER_OF_BOXES = 9;
const guint ROW_1 = 1;
const guint ROW_2 = 2;
const guint ROW_3 = 3;
const guint COLUMN_1 = 1;
const guint COLUMN_2 = 2;
const guint COLUMN_3 = 3;
const guint NUMBER_OF_ROWS = 3;
const guint NUMBER_OF_COLUMNS = 3;
const guint BOX_1 = 1;
const guint BOX_2 = 2;
const guint BOX_3 = 3;
const guint BOX_4 = 4;
const guint BOX_5 = 5;
const guint BOX_6 = 6;
const guint BOX_7 = 7;
const guint BOX_8 = 8;
const guint BOX_9 = 9;
const guint BOX_NEVER_CLICKED = 0;
const guint BOX_CLICKED_ALREADY = 1;

class Board {
  
	public:

	// This is the constructor
	Board (guint);
	// This is the destructor
	~Board (void);

	// Member functions definitions
	void set_box_clicked (guint);
	gboolean get_box_clicked (guint);
	void update_board (guint, guint);
	void display_board (void);
	gboolean is_board_empty(void);
	gboolean is_board_full(void);
	guint** get_current_board (void);
	void reset_board (void);

	private:

	guint gNumOfBoxes;
	gboolean *gBoxClicked;
	guint **gTicTacToeBoard;

};

#endif //BOARD_H
