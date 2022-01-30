#pragma once

#include "Constants.h"

namespace ConsoleChess {

struct Piece {
	char type;
	int color = 0, moved = 0;
	Piece(char t, int c) : type(t), color(c) {}
	Piece(char t) : type(t), color(get_color()) {}
	Piece() : type(EMPTY), color(0), moved(0) {}
	int get_color();
};

}