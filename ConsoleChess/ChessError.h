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
		CantMoveAndKingNotProtected = 8,
		CantMoveToSamePlace = 9,
		PromotePawn = 10
	};

	inline std::string error_to_string(Error err)
	{
		switch (err)
		{
		case ConsoleChess::Unknown:
			return "unknown error";
		case ConsoleChess::Success:
			return "success";
		case ConsoleChess::InvalidMove:
			return "invalid move";
		case ConsoleChess::InvalidTurn:
			return "not your turn to play";
		case ConsoleChess::EmptyPlace:
			return "you must choose a piece to move";
		case ConsoleChess::OutsideBoard:
			return "you must choose a piece to move";
		case ConsoleChess::CantEatKings:
			return "kings cannot be eaten";
		case ConsoleChess::CantEatYourPieces:
			return "can't eat your pieces";
		case ConsoleChess::ProtectYourKing:
			return "you must protect your king";
		case ConsoleChess::CantMoveAndKingNotProtected:
			return "cannot move this, it'll cause a check to your king";
		case ConsoleChess::CantMoveToSamePlace:
			return "cannot move to same place";
		case ConsoleChess::PromotePawn:
			return "promote your pawn";
		default:
			break;
		}
		return "Unknown error";
	}
}