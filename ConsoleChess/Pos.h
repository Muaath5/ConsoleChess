#pragma once

#include <string>
#define A1 Pos('A', 1)
#define H1 Pos('H', 1)
#define A8 Pos('A', 8)
#define H8 Pos('H', 8)
using std::string;

namespace ConsoleChess {

struct Pos {
	char col;
	int row;
	Pos() : row(-1), col('N') {}
	Pos(char c, int r) : row(r), col(c) {}
	Pos(int c, int r) : col(c + 'A'), row(r) {}
	bool operator==(const Pos& r)
	{
		return (this->col == r.col && this->row == r.row);
	}
	string to_string();
};

}