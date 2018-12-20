CC=g++
CFLAGS=-Wall $(shell pkg-config --cflags goocanvas-2.0)
LDFLAGS=$(shell pkg-config --libs goocanvas-2.0)

make: main.cpp game.cpp board.cpp player.cpp
	$(CC) -o tictactoe main.cpp game.cpp board.cpp player.cpp $(CFLAGS) $(LDFLAGS)
	
