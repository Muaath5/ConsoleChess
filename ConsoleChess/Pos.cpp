#include "Pos.h"

namespace ConsoleChess
{

string Pos::to_string()
{
	string s = "";
	s += col;
	s += row + '1';
	return s;
}

}