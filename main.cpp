#include <cstdlib>
#include <cstring>
#include <goocanvas.h>
#include "game.h"
#include "board.h"
#include "player.h"


//Globals
const gint numberOfBoxes = 9;
const gint numberOfPlayers = 2;
const gint PLAYER_1 = 0;
const gint PLAYER_2 = 1;
const gboolean GAME_OVER = 0;
const gboolean GAME_RUNNING = 1;
const gint NO_PLAYER_WIN = 0;
const gint PLAYER_1_WIN = 1;
const gint PLAYER_2_WIN = 2;

volatile gint gCurrentPlayer = PLAYER_1;

GtkWidget *gDialog;
GooCanvasItem *gRoot;
GooCanvasItem **gRect_item;
GooCanvasItem *gInfoBar;
GooCanvasItem *gInfoBarText;
GooCanvasItem *gNought;
GooCanvasItem *gCross;
Board gBoard(numberOfBoxes);
Player gPlayer(numberOfPlayers, gCurrentPlayer);
Game gGame;


//Forward declarations
static gboolean on_delete_event (
	GtkWidget *window, GdkEvent *event, gpointer unused_data);
static gboolean on_rect_button_press (
	GooCanvasItem *view, GooCanvasItem *target, GdkEventButton *event, gpointer data);
void draw_board_gfx(void);
void reset_board_gfx (void);
void draw_info_bar(void);
void init_game (void);
void start_new_game (void);
void update_board_gfx (GooCanvasItem *, gint, gint);
void delete_dynamic_memory(void);


/* This is our handler for the "delete-event" signal of the window, which
   is emitted when the 'x' close button is clicked. We just exit here. */
static gboolean on_delete_event (
	GtkWidget *window, GdkEvent *event, gpointer unused_data) {

	/* Clear up memory */
	delete_dynamic_memory();

	exit (0);
}

/* This handles button presses in item views. We simply output a message to the console */
static gboolean on_rect_button_press (
	GooCanvasItem *view, GooCanvasItem *target, GdkEventButton *event, gpointer data) {

  	// you cast to the type of what you passed as last argument of g_signal_connect
  	gint* pTemp = (gint *)data;
  	gint boxNumber = *pTemp;
  	gint boxNumberIndex = boxNumber - 1;
  	gint playerWinStatus;

  	if (gGame.get_game_status() == GAME_RUNNING) {
	  	if (gBoard.is_board_full() == FALSE) {

		  	//check if the box has been clicked
			if (gBoard.get_box_clicked(boxNumberIndex) == FALSE) {
				gBoard.set_box_clicked(boxNumberIndex);
				
				gCurrentPlayer = gPlayer.get_current_player();

				update_board_gfx (view, gCurrentPlayer, boxNumber);

				gBoard.update_board(boxNumber, gCurrentPlayer);
				gGame.increment_number_of_moves();
				gGame.set_current_board (gBoard.get_current_board());


				if (gBoard.is_board_full() == TRUE) {

					//gGame.display_board();
					gGame.set_game_status (GAME_OVER);

					//check for win	
					playerWinStatus = gGame.check_board_for_win();
					switch (playerWinStatus) {
						case NO_PLAYER_WIN:
							g_object_set(gInfoBarText, "text", "Game over...Draw!", NULL);
							break;
						case PLAYER_1_WIN:
							g_object_set(gInfoBarText, "text", "Game over...Player 1 wins!", NULL);
							break;
						case PLAYER_2_WIN:
							g_object_set(gInfoBarText, "text", "Game over...Player 2 wins!", NULL);
							break;
					}
					gGame.set_game_status(GAME_OVER);

				} else {

					//gGame.display_board();

					//check for win
					playerWinStatus = gGame.check_board_for_win();
					switch (playerWinStatus) {
						case NO_PLAYER_WIN:
							break;
						case PLAYER_1_WIN:
							gGame.set_game_status(GAME_OVER);
							g_object_set(gInfoBarText, "text", "Game over...Player 1 wins!", NULL);
							break;
						case PLAYER_2_WIN:
							gGame.set_game_status(GAME_OVER);
							g_object_set(gInfoBarText, "text", "Game over...Player 2 wins!", NULL);
							break;
					}

					gPlayer.switch_player();
					if (gGame.get_game_status() == GAME_RUNNING) {
						gCurrentPlayer = gPlayer.get_current_player();
						switch (gCurrentPlayer) {
						case PLAYER_1:
							g_object_set(gInfoBarText, "text", "It's Player 1's turn", NULL);
							break;
						case PLAYER_2:
							g_object_set(gInfoBarText, "text", "It's Player 2's turn", NULL);
							break;
						}
					}
				}
			} 
	  	}
	}

	if (gGame.get_game_status() == GAME_OVER) {

		int result = gtk_dialog_run (GTK_DIALOG (gDialog));
		switch (result) {
    		case GTK_RESPONSE_NO:
	       		/* Clear up memory */
				delete_dynamic_memory();
				exit (0);
	       		break;
			case GTK_RESPONSE_YES:
			 	start_new_game ();
				break;
		}
		gtk_widget_hide (gDialog);
	}

	return TRUE;
}

void draw_board_gfx (void) {

	//Top row
	gRect_item[0] = goo_canvas_rect_new (
		gRoot, 0, 0, 200, 200,
		"line-width", 10.0,
		"radius-x", 0.0,
		"radius-y", 0.0,
		"fill-color", "white",
		NULL);

	gRect_item[1] = goo_canvas_rect_new (
		gRoot, 200, 0, 200, 200,
		"line-width", 10.0,
		"radius-x", 0.0,
		"radius-y", 0.0,
		"fill-color", "white",
		NULL);

	gRect_item[2] = goo_canvas_rect_new (
		gRoot, 400, 0, 200, 200,
		"line-width", 10.0,
		"radius-x", 0.0,
		"radius-y", 0.0,
		"fill-color", "white",
		NULL);

	//Middle row
	gRect_item[3] = goo_canvas_rect_new (
		gRoot, 0, 200, 200, 200,
		"line-width", 10.0,
		"radius-x", 0.0,
		"radius-y", 0.0,
		"fill-color", "white",
		NULL);

	gRect_item[4] = goo_canvas_rect_new (
		gRoot, 200, 200, 200, 200,
		"line-width", 10.0,
		"radius-x", 0.0,
		"radius-y", 0.0,
		"fill-color", "white",
		NULL);

	gRect_item[5] = goo_canvas_rect_new (
		gRoot, 400, 200, 200, 200,
		"line-width", 10.0,
		"radius-x", 0.0,
		"radius-y", 0.0,
		"fill-color", "white",
		NULL);

	//Bottom row
	gRect_item[6] = goo_canvas_rect_new (
		gRoot, 0, 400, 200, 200,
		"line-width", 10.0,
		"radius-x", 0.0,
		"radius-y", 0.0,
		"fill-color", "white",
		NULL);

	gRect_item[7] = goo_canvas_rect_new (
		gRoot, 200, 400, 200, 200,
		"line-width", 10.0,
		"radius-x", 0.0,
		"radius-y", 0.0,
		"fill-color", "white",
		NULL);

	gRect_item[8] = goo_canvas_rect_new (
		gRoot, 400, 400, 200, 200,
		"line-width", 10.0,
		"radius-x", 0.0,
		"radius-y", 0.0,
		"fill-color", "white",
		NULL);
}

void reset_board_gfx (void) {

	for (int i = 0; i < numberOfBoxes; i++) {
		g_object_set(gRect_item[i], "fill-color", "white", NULL);
	}
}

void draw_info_bar(void) {

	gInfoBar = goo_canvas_rect_new (
		gRoot, 0, 600, 600, 100,
		"line-width", 10.0,
		"radius-x", 0.0,
		"radius-y", 0.0,
		"fill-color", "light gray",
		NULL);

	gInfoBarText = goo_canvas_text_new (
		gRoot, "New Game...Ready Player 1", 20, 650, 600, 
		GOO_CANVAS_ANCHOR_WEST, 
		"fill-color", "blue",
		"font", "Sans 24",
		NULL);
}

void reset_info_bar(void) {

	g_object_set(gInfoBarText, "text", "New Game...Ready Player 1", NULL);
}

void update_board_gfx (GooCanvasItem *view, gint currentPlayer, gint boxNumber) {

	gint x_pos = 0;
	gint y_pos = 0;

	switch (boxNumber) {
		case 1:
			x_pos = 100;
			y_pos = 100;
			break;
		case 2:
			x_pos = 300;
			y_pos = 100;
			break;
		case 3:
			x_pos = 500;
			y_pos = 100;
			break;
		case 4:
			x_pos = 100;
			y_pos = 300;
			break;
		case 5:
			x_pos = 300;
			y_pos = 300;
			break;
		case 6:
			x_pos = 500;
			y_pos = 300;
			break;
		case 7:
			x_pos = 100;
			y_pos = 500;
			break;
		case 8:
			x_pos = 300;
			y_pos = 500;
			break;
		case 9:
			x_pos = 500;
			y_pos = 500;
			break;
	}

	switch(currentPlayer) {
		case PLAYER_1:
			g_object_set(view, "fill-color", "red", NULL);

			gNought = goo_canvas_text_new (
				gRoot, "O", x_pos, y_pos, 0, 
				GOO_CANVAS_ANCHOR_CENTER, 
				"fill-color", "black",
				"font", "Sans Bold 96",
				NULL);
			break;

		case PLAYER_2:
			g_object_set(view, "fill-color", "yellow", NULL);

			gCross = goo_canvas_text_new (
				gRoot, "X", x_pos, y_pos, 0, 
				GOO_CANVAS_ANCHOR_CENTER, 
				"fill-color", "black",
				"font", "Sans Bold 96",
				NULL);
			break;
	}
}

void init_game (void) {

	draw_board_gfx();
	draw_info_bar();

	gGame.set_game_status(GAME_RUNNING);
	gGame.reset_number_of_moves();
}


void start_new_game (void) {

	reset_board_gfx();
	reset_info_bar();

	gGame.reset_number_of_moves();
	gBoard.reset_board ();
	gGame.set_current_board (gBoard.get_current_board());
	gPlayer.set_current_player (PLAYER_1);
	gGame.set_game_status(GAME_RUNNING);
}


void delete_dynamic_memory (void) {

	/* Clear up memory */
	delete [] gRect_item;
}


int main (int argc, char *argv[]) {

	GtkWidget *window;
	GtkWidget *canvas;
	GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;

	gRect_item = new GooCanvasItem*[numberOfBoxes];
	gint boxNumber[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

	/* Initialize GTK+ */
	gtk_init (&argc, &argv);

	/* Create the window and widgets. */
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size (GTK_WINDOW (window), 600, 600);
	gtk_window_set_resizable (GTK_WINDOW(window), FALSE);
	gtk_window_set_title(GTK_WINDOW(window), "Tic Tac Toe");
	gtk_widget_show (window);
	g_signal_connect (window, "delete_event",  G_CALLBACK (on_delete_event), NULL);

	gDialog = gtk_message_dialog_new (
		GTK_WINDOW(window), flags, GTK_MESSAGE_QUESTION, 
		GTK_BUTTONS_YES_NO, "New Game");

	canvas = goo_canvas_new ();
	gtk_widget_set_size_request (canvas, 600, 700);
	goo_canvas_set_bounds (GOO_CANVAS (canvas), 0, 0, 600, 700);
	gtk_widget_show (canvas);
	gtk_container_add (GTK_CONTAINER (window), canvas);

	gRoot = goo_canvas_get_root_item (GOO_CANVAS (canvas));

	init_game();

	/* Initize signal handlers */
	for (int i = 0; i < numberOfBoxes; i++) {
		g_signal_connect (gRect_item[i], "button_press_event", G_CALLBACK (on_rect_button_press), 
			&boxNumber[i]);
	}

	/* Pass control to the GTK+ main event loop*/
	gtk_main ();

	return 0;
}
