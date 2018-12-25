#include <cstdlib>
#include <cstring>
#include <gtk/gtk.h>
#include "board.h"
#include "player.h"
#include "game.h"


const guint MSG_0 = 0; //Being game...Player 1's turn
const guint MSG_1 = 1; //Player 1's turn
const guint MSG_2 = 2; //Player 2's turn
const guint MSG_3 = 3; //Game over...Draw!
const guint MSG_4 = 4; //Game over...Player 1 wins!
const guint MSG_5 = 5; //Game over...Player 1 wins!
const gdouble M_PI = 3.142; //Pi


GtkWidget *gWindow;
GtkWidget *gNewGameDialog;
GtkWidget *gDrawingArea;
Board gBoard (NUMBER_OF_BOXES);
Player gPlayer (NUMBER_OF_PLAYERS, PLAYER_1);
Game gGame;

gboolean gMenuNewGameFlag;


static gboolean close_window_cb (GtkWidget *, GdkEvent *, gpointer);
static gboolean configure_event_cb (GtkWidget *, GdkEventConfigure *, gpointer);
static gboolean draw_cb (GtkWidget *, cairo_t *, gpointer);
static gboolean button_press_event_cb (GtkWidget *, GdkEventButton *, gpointer);
static void clear_surface (void);
static void draw_grid (GtkWidget *);
static void draw_info_bar (GtkWidget *, guint);
static void draw_noughts_or_crosses (GtkWidget *, guint, gint);
void draw_win_direction(GtkWidget *, guint);
static void advance_game(GtkWidget *, guint);
static void init_game (GtkWidget *);
static void start_new_game (GtkWidget *);


/* Surface to store current scribbles */
static cairo_surface_t *surface = NULL;



static gboolean 
close_window_cb (
	GtkWidget *widget, GdkEvent *event, gpointer data) {

	if (surface) {
		cairo_surface_destroy (surface);
	}
	exit (0);

	/* We've handled the configure event, no need for further processing. */
	return TRUE;
}

/* Create a new surface of the appropriate size to store our scribbles */
static gboolean 
configure_event_cb (
	GtkWidget *widget, GdkEventConfigure *event, gpointer data) {

	if (surface) {
		cairo_surface_destroy (surface);
	}

	surface = gdk_window_create_similar_surface (
		gtk_widget_get_window (widget),
		CAIRO_CONTENT_COLOR,
		gtk_widget_get_allocated_width (widget),
		gtk_widget_get_allocated_height (widget));

	/* Initialize the surface to white */
	clear_surface ();

	/* We've handled the configure event, no need for further processing. */
	return TRUE;
}

/* Redraw the screen from the surface. Note that the ::draw
 * signal receives a ready-to-be-used cairo_t that is already
 * clipped to only draw the exposed areas of the widget
 */
static gboolean 
draw_cb (
	GtkWidget *widget, cairo_t *cr, gpointer data) {

	cairo_set_source_surface (cr, surface, 0, 0);
	cairo_paint (cr);

	return FALSE;
}

/* Handle button press events by either drawing a rectangle
 * or clearing the surface, depending on which button was pressed.
 * The ::button_press signal handler receives a GdkEventButton
 * struct which contains this information.
 */
static gboolean 
button_press_event_cb (
	GtkWidget *widget, GdkEventButton *event, gpointer data) {

	guint x;
	guint y;
	guint rowNumber;
	guint colNumber;
	guint boxSelected;

	/* paranoia check, in case we haven't gotten a configure event */
	if (surface == NULL) {
		return FALSE;
	}

	if (event->button == GDK_BUTTON_PRIMARY) {

		x = static_cast<guint>(event->x);
		y = static_cast<guint>(event->y);

		if ((x >= 0) && (x <= 199)) {
			colNumber = COLUMN_1;
		} else if ((x >= 200) && (x <= 399)) {
			colNumber = COLUMN_2;
		} else if ((x >= 400) && (x <= 600)) {
			colNumber = COLUMN_3;
		}

		if (y <= 600) {
			if ((y >= 0) && (y <= 199)) {
				rowNumber = ROW_1;
			} else if ((y >= 200) && (y <= 399)) {
				rowNumber = ROW_2;
			} else if ((y >= 400) && (y <= 600)) {
				rowNumber = ROW_3;
			}
		} else {
			//y out of range
			return FALSE;
		}

		switch (rowNumber) {
			case ROW_1:
				switch (colNumber) {
					case COLUMN_1:
						boxSelected = BOX_1;
						break;
					case COLUMN_2:
						boxSelected = BOX_2;
						break;
					case COLUMN_3:
						boxSelected = BOX_3;
						break;
					}
					break;
		
			case ROW_2:
				switch (colNumber) {
					case COLUMN_1:
						boxSelected = BOX_4;
						break;
					case COLUMN_2:
						boxSelected = BOX_5;
						break;
					case COLUMN_3:
						boxSelected = BOX_6;
						break;
					}
					break;

			case ROW_3:
				switch (colNumber) {
					case COLUMN_1:
						boxSelected = BOX_7;
						break;
					case COLUMN_2:
						boxSelected = BOX_8;
						break;
					case COLUMN_3:
						boxSelected = BOX_9;
						break;
					}
					break;
		}

		advance_game(widget, boxSelected);
	}

	/* We've handled it, stop processing */
	return TRUE;
}

static void
clear_surface (void) {

	cairo_t *cr;

	cr = cairo_create (surface);

	cairo_set_source_rgb (cr, 1, 1, 1);
	cairo_paint (cr);

	cairo_destroy (cr);
}

static void
draw_grid (GtkWidget *widget) {

	cairo_t *cr;

	/* Paint to the surface, where we store our state */
	cr = cairo_create (surface);

	// Outer border
	cairo_rectangle (cr, 0, 0, 5, 600);
	cairo_fill (cr);
	cairo_rectangle (cr, 0, 0, 600, 5);
	cairo_fill (cr);
	cairo_rectangle (cr, 595, 0, 5, 600);
	cairo_fill (cr);
	cairo_rectangle (cr, 0, 595, 600, 5);
	cairo_fill (cr);

	// Inner grid
	cairo_rectangle (cr, 200-5, 0, 10, 600);
	cairo_fill (cr);
	cairo_rectangle (cr, 400-5, 0, 10, 600);
	cairo_fill (cr);
	cairo_rectangle (cr, 0, 200-5, 600, 10);
	cairo_fill (cr);
	cairo_rectangle (cr, 0, 400-5, 600, 10);
	cairo_fill (cr);

	cairo_destroy (cr);
}

static void
draw_info_bar (GtkWidget *widget, guint messageNumber) {

	cairo_t *cr;

	/* Paint to the surface, where we store our state */
	cr = cairo_create (surface);

	cairo_set_source_rgb(cr, 0.8, 0.8, 0.8);

	cairo_rectangle (cr, 0, 600, 600, 50);
	cairo_fill (cr);

	gtk_widget_queue_draw_area (widget, 0, 600, 600, 50);
	
	cairo_select_font_face(
		cr, "Liberation Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
	cairo_set_font_size(cr, 30);
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_move_to(cr, 20, 635);

	switch (messageNumber) {
		case MSG_0:
			cairo_show_text(cr, "Begin game...Player 1's turn");
			break;
		case MSG_1:
			cairo_show_text(cr, "Player 1's turn");
			break;
		case MSG_2:
			cairo_show_text(cr, "Player 2's turn");
			break;
		case MSG_3:
			cairo_show_text(cr, "Game over...Draw!");
			break;
		case MSG_4:
			cairo_show_text(cr, "Game over...Player 1 wins!");
			break;
		case MSG_5:
			cairo_show_text(cr, "Game over...Player 2 wins!");
	}

	cairo_destroy (cr);
}

static void
draw_noughts_or_crosses(GtkWidget *widget, guint boxSelected, gint currentPlayer) {

	cairo_t *cr;
	gdouble textX;
	gdouble textY;

	/* Paint to the surface, where we store our state */
	cr = cairo_create (surface);

	switch (currentPlayer) {
		case PLAYER_1:
			cairo_set_source_rgb(cr, 1, 0, 0);
			break;
		case PLAYER_2:
			cairo_set_source_rgb(cr, 0, 1, 0);
			break;
	}

	switch (boxSelected) {
		case BOX_1:
			cairo_rectangle (cr, 5, 5, 190, 190);
			cairo_fill (cr);
			gtk_widget_queue_draw_area (widget, 5, 5, 190, 190);
			break;
		case BOX_2:
			cairo_rectangle (cr, 205, 5, 190, 190);
			cairo_fill (cr);
			gtk_widget_queue_draw_area (widget, 205, 5, 190, 190);
			break;
		case BOX_3:
			cairo_rectangle (cr, 405, 5, 190, 190);
			cairo_fill (cr);
			gtk_widget_queue_draw_area (widget, 405, 5, 190, 190);
			break;
		case BOX_4:
			cairo_rectangle (cr, 5, 205, 190, 190);
			cairo_fill (cr);
			gtk_widget_queue_draw_area (widget, 5, 205, 190, 190);
			break;
		case BOX_5:
			cairo_rectangle (cr, 205, 205, 190, 190);
			cairo_fill (cr);
			gtk_widget_queue_draw_area (widget, 205, 205, 190, 190);
			break;
		case BOX_6:
			cairo_rectangle (cr, 405, 205, 190, 190);
			cairo_fill (cr);
			gtk_widget_queue_draw_area (widget, 405, 205, 190, 190);
			break;
		case BOX_7:
			cairo_rectangle (cr, 5, 405, 190, 190);
			cairo_fill (cr);
			gtk_widget_queue_draw_area (widget, 5, 405, 190, 190);
			break;
		case BOX_8:
			cairo_rectangle (cr, 205, 405, 190, 190);
			cairo_fill (cr);
			gtk_widget_queue_draw_area (widget, 205, 405, 190, 190);
			break;
		case BOX_9:
			cairo_rectangle (cr, 405, 405, 190, 190);
			cairo_fill (cr);
			gtk_widget_queue_draw_area (widget, 405, 405, 190, 190);
			break;
	}
	
	cairo_select_font_face(
		cr, "Liberation Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
	cairo_set_font_size(cr, 200);
	cairo_set_source_rgb(cr, 0, 0, 0);


	switch (currentPlayer) {
		case PLAYER_1:
 
			switch (boxSelected) {
				case BOX_1:
					textX = 22;
					textY = 170;
					break;
				case BOX_2:
					textX = 222;
					textY = 170;
					break;
				case BOX_3:
					textX = 422;
					textY = 170;
					break;
				case BOX_4:
					textX = 22;
					textY = 370;
					break;
				case BOX_5:
					textX = 222;
					textY = 370;
					break;
				case BOX_6:
					textX = 422;
					textY = 370;
					break;
				case BOX_7:
					textX = 22;
					textY = 570;
					break;
				case BOX_8:
					textX = 222;
					textY = 570;
					break;
				case BOX_9:
					textX = 422;
					textY = 570;
					break;
			}
			cairo_move_to(cr, textX, textY);
			cairo_show_text(cr, "O"); 
			break;

		case PLAYER_2:

			switch (boxSelected) {
				case BOX_1:
					textX = 35;
					textY = 170;
					break;
				case BOX_2:
					textX = 235;
					textY = 170;
					break;
				case BOX_3:
					textX = 435;
					textY = 170;
					break;
				case BOX_4:
					textX = 35;
					textY = 370;
					break;
				case BOX_5:
					textX = 235;
					textY = 370;
					break;
				case BOX_6:
					textX = 435;
					textY = 370;
					break;
				case BOX_7:
					textX = 35;
					textY = 570;
					break;
				case BOX_8:
					textX = 235;
					textY = 570;
					break;
				case BOX_9:
					textX = 435;
					textY = 570;
					break;
			}
			cairo_move_to(cr, textX, textY);
			cairo_show_text(cr, "X");
			break;
	}

	cairo_destroy (cr);

	/* Now invalidate the affected region of the drawing area */
	gtk_widget_queue_draw_area (widget, 5, 5, 190, 190);
}

void draw_win_direction(GtkWidget *widget, guint winDirection) {

	if (winDirection != NO_CURRENT_WIN) {

		cairo_t *cr;

		/* Paint to the surface, where we store our state */
		cr = cairo_create (surface);

		cairo_set_source_rgb(cr, 1, 1, 1);

		switch (winDirection) {
			case NO_CURRENT_WIN:
				break;
			case TOP_ROW:
				//g_print ("Draw line across TOP_ROW\n");
				cairo_rectangle (cr, 10, 90, 580, 20);
				cairo_fill (cr);
				gtk_widget_queue_draw_area (widget, 0, 0, 600, 600);
				break;
			case MIDDLE_ROW:
				//g_print ("Draw line across MIDDLE_ROW\n");
				cairo_rectangle (cr, 10, 290, 580, 20);
				gtk_widget_queue_draw_area (widget, 0, 0, 600, 600);
				cairo_fill (cr);
				break;
			case BOTTOM_ROW:
				//g_print ("Draw line across BOTTOM_ROW\n");
				cairo_rectangle (cr, 10, 490, 580, 20);
				cairo_fill (cr);
				gtk_widget_queue_draw_area (widget, 0, 0, 600, 600);
				break;
			case LEFT_COLUMN:
				//g_print ("Draw line across LEFT_COLUMN\n");
				cairo_rectangle (cr, 90, 10, 20, 580);
				cairo_fill (cr);
				gtk_widget_queue_draw_area (widget, 0, 0, 600, 600);
				break;
			case MIDDLE_COLUMN:
				//g_print ("Draw line across MIDDLE_COLUMN\n");
				cairo_rectangle (cr, 290, 10, 20, 580);
				cairo_fill (cr);
				gtk_widget_queue_draw_area (widget, 0, 0, 600, 600);
				break;
			case RIGHT_COLUMN:
				//g_print ("Draw line across RIGHT_COLUMN\n");
				cairo_rectangle (cr, 490, 10, 20, 580);
				cairo_fill (cr);
				gtk_widget_queue_draw_area (widget, 0, 0, 600, 600);
				break;
			case TOP_LEFT_TO_BOTTOM_RIGHT:
				//g_print ("Draw line across TOP_LEFT_TO_BOTTOM_RIGHT\n");
        cairo_rotate (cr, 45*(M_PI/180));
				cairo_fill (cr);
				cairo_rectangle (cr, 53, -10, 748, 20);
				cairo_fill (cr);
				gtk_widget_queue_draw_area (widget, 0, 0, 600, 600);
				break;
			case TOP_RIGHT_TO_BOTTOM_LEFT:
				//g_print ("Draw line across TOP_RIGHT_TO_BOTTOM_LEFT\n");
        cairo_rotate (cr, -45*(M_PI/180));
				cairo_fill (cr);
				//                   left/right, up/down																					
				cairo_rectangle (cr, -375, 415, 748, 20);
				cairo_fill (cr);
				gtk_widget_queue_draw_area (widget, 0, 0, 600, 600);
				break;
		}
		cairo_destroy (cr);
	}
}

static void init_game (GtkWidget *drawing_area) {

	//GUI
	draw_grid(drawing_area);
	draw_info_bar (drawing_area, MSG_0);

	//Logic
	gGame.set_game_status (GAME_RUNNING);
	gGame.reset_number_of_moves();
	gMenuNewGameFlag = FALSE;
}

static void start_new_game (GtkWidget *widget) {

	//Reset GUI
	clear_surface ();
	draw_grid(widget);
	draw_info_bar (widget, MSG_0);

	//Reset logic
	gGame.reset_number_of_moves();
	gGame.reset_current_board();
	gBoard.reset_board();
	gPlayer.reset_player_moves ();
	gPlayer.set_current_player (PLAYER_1);
	gGame.set_game_status(GAME_RUNNING);
	gGame.reset_win_direction();
}

static void advance_game(GtkWidget *widget, guint boxSelected) {

	guint currentPlayer;
	guint playerWinStatus;
	guint **tmp;
	guint winDirection;


	if (gGame.get_game_status() == GAME_RUNNING) {

		if (gBoard.is_board_full() == BOARD_NOT_FULL) {

			if (gBoard.get_box_clicked (boxSelected) == BOX_NEVER_CLICKED) {

				gBoard.set_box_clicked(boxSelected);

				currentPlayer = gPlayer.get_current_player();
				gPlayer.record_player_move (
					gGame.get_number_of_moves(), currentPlayer, boxSelected);

				gBoard.update_board (boxSelected, currentPlayer);

				draw_noughts_or_crosses(widget, boxSelected, currentPlayer);

				gGame.increment_number_of_moves();
				tmp = gBoard.get_current_board ();
				gGame.set_current_board (tmp);

				if (gBoard.is_board_full() == BOARD_IS_FULL) {

					//gGame.display_board();
					gGame.set_game_status (GAME_OVER);

					//check for win	
					playerWinStatus = gGame.check_board_for_win();
					if (playerWinStatus != NO_WIN) {
						switch (playerWinStatus) {
							case NO_PLAYER_WIN:
								draw_info_bar (widget, MSG_3);
								break;
							case PLAYER_1_WIN:
								draw_info_bar (widget, MSG_4);
								break;
							case PLAYER_2_WIN:
								draw_info_bar (widget, MSG_5);
								break;
						}
						winDirection = gGame.get_win_direction();
						draw_win_direction(widget, winDirection);
					}
					gGame.set_game_status(GAME_OVER);

				} else {

					//gGame.display_board();

					//check for win
					playerWinStatus = gGame.check_board_for_win();
					if (playerWinStatus != NO_WIN) {
						switch (playerWinStatus) {
							case NO_PLAYER_WIN:
								break;
							case PLAYER_1_WIN:
								gGame.set_game_status(GAME_OVER);
								draw_info_bar (widget, MSG_4);

								break;
							case PLAYER_2_WIN:
								gGame.set_game_status(GAME_OVER);
								draw_info_bar (widget, MSG_5);
								break;
						}
						winDirection = gGame.get_win_direction();
						draw_win_direction(widget, winDirection);
					}
				}

				gPlayer.switch_player();
				if (gGame.get_game_status() == GAME_RUNNING) {
					currentPlayer = gPlayer.get_current_player();
					switch (currentPlayer) {
					case PLAYER_1:
						draw_info_bar (widget, MSG_1);
						break;
					case PLAYER_2:
						draw_info_bar (widget, MSG_2);
						break;
					}
				}

			}
		}
	}

	if (gGame.get_game_status() == GAME_OVER) {

		guint state = TRUE;

		while (state) {
		int result = gtk_dialog_run (GTK_DIALOG (gNewGameDialog));
		switch (result) {

			case GTK_RESPONSE_DELETE_EVENT:

				//signal was given by File->New Game
				if (gMenuNewGameFlag == TRUE) {
					gGame.set_game_status(GAME_RUNNING);
					gMenuNewGameFlag = FALSE;
					state = FALSE;
				} else { //signal was given by end of game
					gMenuNewGameFlag = FALSE;
					state = TRUE;
				}

				break;

			case GTK_RESPONSE_YES:
				state = FALSE;
			 	start_new_game(widget);
				break;

    		case GTK_RESPONSE_NO:
    			state = FALSE;
					g_signal_emit_by_name (gWindow, "destroy");
	       	break;
			}
		}

		gtk_widget_hide (gNewGameDialog);
	}
	//make a global copy
	gDrawingArea = widget;
}



static void
new_game_cb (GSimpleAction *action,
            GVariant      *parameter,
            gpointer       user_data)
{
	gGame.set_game_status(GAME_OVER);
	gMenuNewGameFlag = TRUE;
	advance_game(gDrawingArea, 0);
}

static void
about_cb (GSimpleAction *action,
            GVariant      *parameter,
            gpointer       user_data)
{
	GdkPixbuf *tictactoe_logo = 
		gdk_pixbuf_new_from_file ("/usr/local/share/tictactoe_icon.png", NULL);

	gtk_show_about_dialog (
		NULL,
		"program-name", "Tic-tac-toe",
		"title", "About Tic-tac-toe",
		"logo", tictactoe_logo,
		"comments", "A simple noughts and crosses game.",
		NULL);
}

static void
exit_cb (GSimpleAction *action,
            GVariant      *parameter,
            gpointer       user_data)
{
	g_signal_emit_by_name (gWindow, "destroy");
}

static GActionEntry app_entries[] = {
	{ "newGame", new_game_cb, NULL, NULL, NULL },
  	{ "about", about_cb, NULL, NULL, NULL },
  	{ "exit", exit_cb, NULL, NULL, NULL }
};

static void
activate (GtkApplication *app, gpointer user_data) {

	GtkWidget *frame;
	GtkWidget *drawing_area;

	GMenu *file_menu;
 	GMenu *s1, *s2;

	GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;

	gWindow = gtk_application_window_new (app);
	gtk_window_set_title (GTK_WINDOW (gWindow), "Tic-tac-toe");
	gtk_window_set_resizable (GTK_WINDOW (gWindow), FALSE);

	g_signal_connect (gWindow, "destroy", G_CALLBACK (close_window_cb), &app);

	gtk_container_set_border_width (GTK_CONTAINER (gWindow), 8);

	frame = gtk_frame_new (NULL);
	gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_IN);
	gtk_container_add (GTK_CONTAINER (gWindow), frame);

	/* Create the drawing area */
	drawing_area = gtk_drawing_area_new ();
	gtk_widget_set_size_request (drawing_area, 600, 650);

	gtk_container_add (GTK_CONTAINER (frame), drawing_area);

	gNewGameDialog = gtk_message_dialog_new (GTK_WINDOW(gWindow), flags, 
		GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "New Game");

	/* Signals used to handle backing surface */
	g_signal_connect (drawing_area, "draw",	G_CALLBACK (draw_cb), NULL);
	g_signal_connect (drawing_area, "configure_event", G_CALLBACK (configure_event_cb), NULL);

	/* Event signals */
	g_signal_connect (drawing_area, "button_press_event", G_CALLBACK (button_press_event_cb), NULL);

	/* Ask to receive events the drawing doesn't normally subscribe to. In particular, 
		we need to ask for the button press and motion notify events that want to handle. */
	gtk_widget_set_events (
		drawing_area, gtk_widget_get_events (drawing_area) |
		GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK);


	g_action_map_add_action_entries (
		G_ACTION_MAP (G_APPLICATION (app)), app_entries, G_N_ELEMENTS (app_entries), G_APPLICATION (app));

	if (g_getenv ("APP_MENU_FALLBACK")) {
    	g_object_set (gtk_settings_get_default (), "gtk-shell-shows-app-menu", FALSE, NULL);
	}

	file_menu = g_menu_new ();
 	
 	s1 = g_menu_new ();
 	g_menu_append (s1, "New Game", "app.newGame");
 	g_menu_append (s1, "About", "app.about");
 	g_menu_append_section (file_menu, NULL, G_MENU_MODEL(s1));
 
	s2 = g_menu_new ();
 	g_menu_append (s2, "Exit", "app.exit");
 	g_menu_append_section (file_menu, NULL, G_MENU_MODEL(s2));

	gtk_application_set_app_menu (GTK_APPLICATION (app), G_MENU_MODEL (file_menu));
	g_object_unref (file_menu);


	gtk_widget_show_all (gWindow);

	init_game(drawing_area);
}

int main (int argc, char **argv) {

	GtkApplication *app;
	int status;


	/* Since this example is running uninstalled,
	* we have to help it find its schema. This
	* is *not* necessary in properly installed
	* application.
	*/
	g_setenv ("GSETTINGS_SCHEMA_DIR", ".", FALSE);

	app = gtk_application_new ("org.gtk.tictactoe", G_APPLICATION_FLAGS_NONE);
	g_signal_connect (app, "activate", G_CALLBACK(activate), NULL);
	status = g_application_run (G_APPLICATION (app), argc, argv);
	g_object_unref (app);

	return status;
}
