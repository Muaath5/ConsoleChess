// Copyright 2022
// Written by: mdubaisi, Muaath_5

#include "Chess.h"

namespace ConsoleChess {

inline bool Board::will_get_check(Pos king, Pos from, Pos to)
{
	move_piece_internal(from, to, false, "");
	bool res = is_checked(king);
	undo_last_move();
	return res;
}

Error Board::move_piece(Pos from, Pos to)
{
	// Input validation
	if (!is_valid_pos(from) || !is_valid_pos(to))
		return Error::OutsideBoard;

	if (from == to)
		return Error::CantMoveToSamePlace;

	// {from} & {to} must be zero based
	from.row--, to.row--;
	int from_x = from.col - 'A', from_y = from.row;
	int to_x = to.col - 'A', to_y = to.row;
	Piece& piece = get_piece(from);
	Piece& newPlace = get_piece(to);

	// Checks if it was correct turn
	if (piece.color == 0)
		return Error::EmptyPlace;
		//error("the place you choosed is empty!");

	if (piece.color != turn) 
		return Error::InvalidTurn;
		//error("you can only move your own pieces!");

	if (newPlace.color == turn)
		return Error::CantEatYourPieces;
		//error("can't eat your pieces!");

	if (tolower(newPlace.type) == B_KING)
		return Error::CantEatKings;

	Pos king = (turn == BLACK ? blackKing : whiteKing);

	if (false && will_get_check(king, from, to))
		return Error::CantMoveAndKingNotProtected;

	
	if (false && is_checked(king) && !will_remove_check(king, from, to))
	{
		return Error::ProtectYourKing;
	}

	switch (piece.type) {
	case W_PAWN:
		if ((!piece.moved && from_x == to_x && to_y == from_y + 2) || (from_x == to_x && to_y == from_y + 1) ||
			(abs(from_x - to_x) == 1 && to_y == from_y - 1 && (board[to_y][to_x].color == BLACK || (from_y == 3 && board[from_y][to_x].color == BLACK && board[from_y][to_x].moved == 1)))) {
			bool enPassent = false;
			if (from_y == 3 && board[from_y][to_x].color == BLACK && board[from_y][to_x].moved == 1)
				enPassent = true;
			move_piece_internal(from, to, enPassent);
			if (to_y == 0)
				return Error::PromotePawn;
			break;
		}
		return Error::InvalidMove;
	case B_PAWN:
		if ((!piece.moved && from_x == to_x && to_y == from_y - 2) || (from_x == to_x && to_y == from_y - 1) ||
			(abs(from_x - to_x) == 1 && to_y == from_y + 1 && (board[to_y][to_x].color == WHITE || (from_y == 4 && board[from_y][to_x].color == WHITE && board[from_y][to_x].moved == 1)))) {
			
			bool enPassent = false;
			if (from_y == 4 && board[from_y][to_x].color == WHITE && board[from_y][to_x].moved == 1)
				enPassent = true;
			move_piece_internal(from, to, enPassent);
			if (to_y == BOARD_SIZE - 1)
				return Error::PromotePawn;
			break;
		}
		return Error::InvalidMove;

	case W_KNIGHT:
	case B_KNIGHT:
		if (can_knight_take(from, to)) {
			move_piece_internal(from, to);
			break;
		}
		return Error::InvalidMove;

	case W_BISHOP:
	case B_BISHOP:
		if (can_bishop_take(from, to)) {
			move_piece_internal(from, to);
			break;
		}
		return Error::InvalidMove;

	case W_ROOK:
	case B_ROOK:
		if (can_rook_take(from, to)) {
			move_piece_internal(from, to);
			break;
		}
		return Error::InvalidMove;

	case W_QUEEN:
	case B_QUEEN:
		if (can_bishop_take(from, to) || can_rook_take(from, to)) {
			move_piece_internal(from, to);
			break;
		}
		return Error::InvalidMove;

	case B_KING:
	case W_KING:
		if ((from.row == to.row + 1 || from.row == to.row - 1 || from.row == to.row) &&
			(from_x == from_y - 1 || from_x == from_y || from_x == from_y + 1))
		{
			move_piece_internal(from, to);
			break;
		}
		return Error::InvalidMove;
	default:
		return Error::Unknown;
	}

	return Error::Success;
}

inline void Board::move_piece_internal(Pos from, Pos to, bool enPassent, string new_history)
{
	Piece& piece = get_piece(from);
	piece.moved++;
	Piece cpy = piece;
	Piece& eat = get_piece(to);
	if (enPassent)
		eat = get_piece(Pos(to.col, to.row + turn)); // + turn because it'll be -1 if black, +1 in white
	history.push_back(Move(from, to, new_history, eat));
	eat = cpy;
	piece = Piece();
	
	if (piece.type == B_KING) blackKing = to;
	if (piece.type == W_KING) whiteKing = to;
	next_turn();
}

inline bool Board::will_remove_check(Pos king, Pos from, Pos to)
{
	move_piece_internal(from, to);
	bool res = !is_checked(king);
	undo_last_move();
	return res;
}

bool Board::king_side_castle()
{
	Pos kingPos = (turn == BLACK ? blackKing : whiteKing);
	int row = kingPos.row - 1;
	kingPos.row = row;
	Piece king = get_piece(kingPos);
	Pos rookPos = kingPos;
	rookPos.col = 'H';
	Piece rook = get_piece(rookPos);
	// If they didn't move and all of between them is empty
	if (!rook.moved && !king.moved &&
		get_piece(Pos('F', row)).type == EMPTY &&
		get_piece(Pos('G', row)).type == EMPTY &&
		(char)tolower(rook.type) == B_ROOK && (char)tolower(king.type) == B_KING)
	{
		move_piece_internal(rookPos, Pos('F', row), false, "");
		move_piece_internal(kingPos, Pos('G', row), false, "O-O");
		next_turn();
		return true;
	}
	return false;
}

bool Board::queen_side_castle()
{
	Pos kingPos = (turn == BLACK ? blackKing : whiteKing);
	int row = kingPos.row - 1;
	kingPos.row = row;
	Piece king = get_piece(kingPos);
	Pos rookPos = kingPos;
	rookPos.col = 'H';
	Piece rook = get_piece(rookPos);
	// If they didn't move and all of between them is empty
	if (!rook.moved && !king.moved &&
		get_piece(Pos('B', row)).type == EMPTY &&
		get_piece(Pos('C', row)).type == EMPTY &&
		get_piece(Pos('D', row)).type == EMPTY &&
		(char)tolower(rook.type) == B_ROOK && (char)tolower(king.type) == B_KING)
	{
		move_piece_internal(rookPos, Pos('D', row), false, "");
		move_piece_internal(kingPos, Pos('C', row), false, "O-O-O");
		next_turn();
		return true;
	}
	return false;
}

string Board::print_board() {
	string boardStr = "";
	if (history_mode != HistoryMode::Disable)
	{
		boardStr += "History: ";
		if (history_mode == HistoryMode::Positions || history_mode == HistoryMode::Both)
			boardStr += get_history() + endl;
		if (history_mode == HistoryMode::FEN || history_mode == HistoryMode::Both)
			boardStr += get_fen() + endl;
		boardStr += endl;
	}
	for (int i = BOARD_SIZE - 1; i >= 0; i--) {
		boardStr += (i + 1 + '0'), boardStr += "  ";
		for (int j = 0; j < BOARD_SIZE; j++)
			boardStr += board[j][i].type, boardStr += " ";
		boardStr += endl;
	}

	boardStr += endl, boardStr += "  ";
	for (int i = 0; i < BOARD_SIZE; i++)
		boardStr += " ", boardStr += (char)('A' + i);
	boardStr += endl;
	return boardStr;
}

string Board::get_fen()
{
	string fen = "";
	bool started = false;
	for (int i = BOARD_SIZE-1; i >= 0; i--)
	{
		if (started)
			fen += '/';
		int cnt = 0;
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			cnt++;
			if (board[j][i].type == EMPTY)
				continue;
			if (cnt > 1)
				fen += cnt+'0';
			fen += board[j][i].type;
			cnt = 0;
		}
		if (cnt > 0)
			fen += cnt + '0';
		started = true;
	}
	return fen;
}

void Board::initialize(GameMode mode)
{
	const int blackRow = BOARD_SIZE - 1;
	const int whiteRow = 0;
	for (int x = 0; x < BOARD_SIZE; x++) {
		board[x][whiteRow + 1] = Piece(W_PAWN);
		board[x][blackRow - 1] = Piece(B_PAWN);
	}
	board[0][whiteRow] = Piece(W_ROOK); board[BOARD_SIZE - 1][whiteRow] = Piece(W_ROOK);
	board[1][whiteRow] = Piece(W_KNIGHT); board[BOARD_SIZE - 2][whiteRow] = Piece(W_KNIGHT);
	board[2][whiteRow] = Piece(W_BISHOP); board[BOARD_SIZE - 3][whiteRow] = Piece(W_BISHOP);
	board[3][whiteRow] = Piece(W_QUEEN); board[BOARD_SIZE - 4][whiteRow] = Piece(W_KING);

	// Placing in the two sides
	board[0][blackRow] = Piece(B_ROOK); board[BOARD_SIZE - 1][blackRow] = Piece(B_ROOK);
	board[1][blackRow] = Piece(B_KNIGHT); board[BOARD_SIZE - 2][blackRow] = Piece(B_KNIGHT);
	board[2][blackRow] = Piece(B_BISHOP); board[BOARD_SIZE - 3][blackRow] = Piece(B_BISHOP);
	board[3][blackRow] = Piece(B_QUEEN); board[BOARD_SIZE - 4][blackRow] = Piece(B_KING);

	if (mode == GameMode::Mode960)
	{
		vector<Piece> row;
		for (int i = 0; i < BOARD_SIZE; i++)
		{
			row.push_back(board[i][whiteRow]);
		}
		std::random_shuffle(row.begin(), row.end());
		for (int i = 0; i < BOARD_SIZE; i++)
		{
			board[i][whiteRow] = row[i];
			board[i][blackRow] = row[i];
			board[i][blackRow].color = BLACK;
			board[i][blackRow].type = (char)tolower(board[i][blackRow].type);
		}
	}
}

}