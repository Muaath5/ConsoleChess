// Copyright 2022
// Written by: mdubaisi, Muaath_5

#include "Chess.h"

namespace ConsoleChess {

inline bool Board::will_get_check(Pos king, Pos from, Pos to)
{
	move_piece_internal(from, to, false, "", false);
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

	if (piece.color != turn) 
		return Error::InvalidTurn;

	if (newPlace.color == turn)
		return Error::CantEatYourPieces;

	if (tolower(newPlace.type) == B_KING)
		return Error::CantEatKings;

	Pos king = (turn == BLACK ? blackKing : whiteKing);

	if (will_get_check(king, from, to))
		return Error::KingWillBeChecked;

	
	if (is_checked(king))
	{
		if (!will_remove_check(king, from, to))
			return Error::ProtectYourKing;
	}

	switch (piece.type) {
	case W_PAWN:
		if (
			(!piece.moved && from_x == to_x && to_y == from_y + 2) ||
			(from_x == to_x && to_y == from_y + 1) ||
			(abs(from_x - to_x) == 1 && to_y == from_y + 1) ||
			(from_y == 3 && board[from_y][to_x].moved == 1)) {

			bool enPassent = false;
			if (from_y == 3 && board[from_y][to_x].moved == 1)
				enPassent = true;
			move_piece_internal(from, to, enPassent);
			if (to_y == 0)
				return Error::PromotePawn;
			break;
		}
		return Error::InvalidMove;
	case B_PAWN:

		if (
			(!piece.moved && from_x == to_x && to_y == from_y - 2) || // Two blocks forward
			(from_x == to_x && to_y == from_y - 1) || // Moving forward
			(abs(from_x - to_x) == 1 && to_y == from_y - 1) || // Eating
			(from_y == 4 && board[from_y][to_x].moved == 1)) // En Passent
		{
			
			bool enPassent = false;
			if (from_y == 4 && board[from_y][to_x].moved == 1)
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

inline void Board::move_piece_internal(Pos from, Pos to, bool enPassent, string new_history, bool real)
{
	Piece& piece = get_piece(from);
	Piece& eat = get_piece(to);
	if (real)
	{
		if (turn == BLACK) // after swaping
			full_moves++;
		if (eat.type != EMPTY || tolower(piece.type) == B_PAWN)
			halfmove_clock = 0;
		else
			halfmove_clock++;

		piece.moved++;
	}
	Piece cpy = piece;
	if (enPassent)
		eat = get_piece(Pos(to.col, to.row + turn)); // + turn because it'll be -1 if black, +1 in white

	auto mv = Move(from, to, (enPassent ? MoveType::EnPassent : MoveType::Normal));
	mv.eaten = eat;
	history.push_back(mv);
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

Error Board::move(Move move)
{
	if (move.type == MoveType::KingSideCastle)
	{
		if (!castle_king_side())
		{
			return Error::CastlingKingSideNotAllowed;
		}
		return Error::Success;
	}
	if (move.type == MoveType::QueenSideCastle)
	{
		if (!castle_queen_side())
		{
			return Error::CastlingQueenSideNotAllowed;
		}
		return Error::Success;
	}
	return move_piece(move.from, move.to);
}

bool Board::castle_king_side()
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
bool Board::castle_queen_side()
{
	Pos kingPos = (turn == BLACK ? blackKing : whiteKing);
	int row = kingPos.row - 1;
	kingPos.row = row;
	Piece king = get_piece(kingPos);
	Pos rookPos = kingPos;
	rookPos.col = 'H';
	Piece rook = get_piece(rookPos);
	// If they didn't move and all of between them is empty
	if (can_castle_queen_side(turn))
	{
		move_piece_internal(rookPos, Pos('D', row), false, "");
		move_piece_internal(kingPos, Pos('C', row), false, "O-O-O");
		next_turn();
		return true;
	}
	return false;
}

bool Board::can_castle_queen_side(int color)
{
	Pos kingPos = (color == BLACK ? blackKing : whiteKing);
	int row = kingPos.row - 1;
	kingPos.row = row;
	Piece king = get_piece(kingPos);
	Pos rookPos = kingPos;
	rookPos.col = 'H';
	Piece rook = get_piece(rookPos);
	return (!rook.moved && !king.moved &&
		get_piece(Pos('B', row)).type == EMPTY &&
		get_piece(Pos('C', row)).type == EMPTY &&
		get_piece(Pos('D', row)).type == EMPTY &&
		(char)tolower(rook.type) == B_ROOK && (char)tolower(king.type) == B_KING);
}
bool Board::can_castle_king_side(int color)
{
	Pos kingPos = (turn == color ? blackKing : whiteKing);
	int row = kingPos.row - 1;
	kingPos.row = row;
	Piece king = get_piece(kingPos);
	Pos rookPos = kingPos;
	rookPos.col = 'H';
	Piece rook = get_piece(rookPos);
	return (!rook.moved && !king.moved &&
		get_piece(Pos('F', row)).type == EMPTY &&
		get_piece(Pos('G', row)).type == EMPTY &&
		(char)tolower(rook.type) == B_ROOK && (char)tolower(king.type) == B_KING);
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
			cnt--;
			if (cnt > 0)
				fen += cnt+'0';
			fen += board[j][i].type;
			cnt = 0;
		}
		if (cnt > 0)
			fen += cnt + '0';
		started = true;
	}
	fen += (turn == WHITE ? " w " : " b ");
	// If can castle
	string castling = "";
	if (!get_piece(whiteKing).moved)
	{
		if (!get_piece(A1).moved) castling += "K";
		if (!get_piece(H1).moved) castling += "Q";
	}
	if (!get_piece(whiteKing).moved)
	{
		if (!get_piece(A1).moved) castling += "k";
		if (!get_piece(H1).moved) castling += "q";
	}

	if (castling == "")
		fen += "-";
	else
		fen += castling + " ";

	// If Enpassent is available
	fen += "-";

	fen += " " + std::to_string(halfmove_clock);

	fen += " " + std::to_string(full_moves);
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

bool Board::is_stalemate()
{
	// We must scan all pieces if can they move
	return false;
}

bool Board::is_draw_by_repeation()
{
	// We must check history
	return false;
}

}