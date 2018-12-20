#ifndef PLAYER_H
#define PLAYER_H

#include <goocanvas.h>

class Player {
  
	public:

	// This is the constructor
	Player (gint, gint);
	// This is the destructor
	~Player (void);

	// Member functions definitions
	void get_player_info (void);
	void set_current_player (gint);
	gint get_current_player (void);
	void switch_player (void);

	private:

	gint gNumberOfPlayers;
	gint gCurrentPlayer;

};

#endif //PLAYER_H