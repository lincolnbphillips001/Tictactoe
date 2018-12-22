#ifndef PLAYER_H
#define PLAYER_H

#include <gtk/gtk.h>

const gint NUMBER_OF_PLAYERS = 2;
const gint PLAYER_1 = 0;
const gint PLAYER_2 = 1;
const gint NO_PLAYER_WIN = 0;
const gint PLAYER_1_WIN = 1;
const gint PLAYER_2_WIN = 2;

typedef struct PlayerMoveRecord {
	guint player;
	guint boxSelected;
} PlayerMoveRecord;

class Player {
  
	public:

	// This is the constructor
	Player (guint, guint);
	// This is the destructor
	~Player (void);

	// Member functions definitions
	void set_current_player (guint);
	guint get_current_player (void);
	void switch_player (void);
	void record_player_move (guint, guint, guint);
	PlayerMoveRecord* get_player_moves (void);
	void reset_player_moves (void);

	private:

	guint gNumberOfPlayers;
	guint gCurrentPlayer;
	PlayerMoveRecord *gPlayerMoveRecord;
	guint gPlayerMoveRecordIndex;
};

#endif //PLAYER_H
