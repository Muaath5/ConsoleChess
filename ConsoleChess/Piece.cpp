#include "Piece.h"

namespace ConsoleChess {

int Piece::get_color()
{
	return (type >= 'a' ? BLACK : WHITE);
}

}