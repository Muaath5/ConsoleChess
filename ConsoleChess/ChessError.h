#pragma once
#include <string>

namespace ConsoleChess {
	enum Error {
		Unknown = -1,
		Success = 0,
		InvalidMove = 1,
		InvalidTurn = 2,
		EmptyPlace = 3,
		OutsideBoard = 4,
		CantEatKings = 5,
		CantEatYourPieces = 6,
		ProtectYourKing = 7,
		KingWillBeChecked = 8,
		CantMoveToSamePlace = 9,
		PromotePawn = 10,
		CastlingKingSideNotAllowed = 11,
		CastlingQueenSideNotAllowed = 12
	};

	inline std::string error_to_string(Error err)
	{
		switch (err)
		{
		case ConsoleChess::Unknown:
			return "Unknown error";
		case ConsoleChess::Success:
			return "success";
		case ConsoleChess::InvalidMove:
			return "invalid move";
		case ConsoleChess::InvalidTurn:
			return "not your turn to play";
		case ConsoleChess::EmptyPlace:
			return "you must choose a piece to move";
		case ConsoleChess::OutsideBoard:
			return "outside of the board!";
		case ConsoleChess::CantEatKings:
			return "kings cannot be eaten";
		case ConsoleChess::CantEatYourPieces:
			return "can't eat your pieces";
		case ConsoleChess::ProtectYourKing:
			return "you must protect your king";
		case ConsoleChess::KingWillBeChecked:
			return "cannot move this, it'll cause a check to your king";
		case ConsoleChess::CantMoveToSamePlace:
			return "cannot move to same place";
		case ConsoleChess::PromotePawn:
			return "promote your pawn";
		case ConsoleChess::CastlingQueenSideNotAllowed:
			return "castling queen side not allowed";
		case ConsoleChess::CastlingKingSideNotAllowed:
			return "castling king side not allowed";
		default:
			break;
		}
		return "Error is unknown!";
	}
}