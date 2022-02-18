#pragma once
#include <string>
#include "Piece.h"
#include "Pos.h";

namespace ConsoleChess {

enum MoveType {
	Normal = 0,
	EnPassent,
	QueenSideCastle,
	KingSideCastle
};

struct Move {
	// This can be set only by developer
	const static bool uci_notation = false;

	MoveType type = MoveType::Normal;
	Pos from=Pos(), to=Pos();
	Piece eaten = Piece(); // Only useful when going back to be used as history
	string notation = "\0";
	Move() {}
	Move(Pos f, Pos t) : from(f), to(t) { notation = Move::get_notation(f, t); }
	Move(MoveType t) : type(t) {}
	Move(Pos f, Pos t, MoveType type) : from(f), to(t), type(type) 
	{
		if (!uci_notation)
		{
			if (type == MoveType::QueenSideCastle)
			{
				notation = "O-O-O";
			}
			if (type == MoveType::KingSideCastle)
			{
				notation = "O-O";
			}
		}
		else
		{
			notation = Move::get_notation(f, t);
		}
	}
	Move(string notation) : notation(notation)
	{
		from = Pos((char)toupper(notation[0]), notation[1] - '0');
		to = Pos((char)toupper(notation[2]), notation[3] - '0');
		if (notation == "e1g1" || notation == "e8g8" || notation == "O-O")
		{
			type = MoveType::KingSideCastle;
			if (!uci_notation) notation = "O-O";
		}
		if (notation == "e1c1" || notation == "e8c8" || notation == "O-O-O")
		{
			type = MoveType::QueenSideCastle;
			if (!uci_notation) notation = "O-O-O";
		}
	}
	static string get_notation(Pos from, Pos to)
	{
		return from.to_string() + to.to_string();
	}
};

}