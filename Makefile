CC=g++
CFLAGS=-Wall $(shell pkg-config --cflags gtk+-3.0)
LDFLAGS=$(shell pkg-config --libs gtk+-3.0)

make: main.cpp board.cpp player.cpp game.cpp
	$(CC) -o TicTacToeGUIGtk3 main.cpp board.cpp player.cpp game.cpp $(CFLAGS) $(LDFLAGS)
	
