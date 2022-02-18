#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include "Piece.h"
#include "Constants.h"
#include "ChessError.h"
#include "Pos.h"
#include "Move.h"

#define endl '\n'

#define INVALID_PIECE_POS Pos('N', -1)

using std::string;
using std::vector;

enum GameMode { Normal = 0, Mode960 };
enum HistoryMode { Disable = -1, Positions = 0, FEN = 1, Both = 2 };
enum GameResult { StillPlaying = -10, WhiteCheckmated = -1, Draw = 0, BlackCheckmated = 1, Stalemate = 10, DrawByRepeatation = 11, DrawBy50Move = 12 };

namespace ConsoleChess {

	struct Board {
	public:
		Board(GameMode mode = GameMode::Normal) {
			initialize(mode);
		}
	protected:
		// grid[0][0] == A1 == bottom-left
		Piece board[BOARD_SIZE][BOARD_SIZE];

		// King positions, used for getting checkmates
		Pos whiteKing = Pos('E', 1), blackKing = Pos('E', 8);

		inline void move_piece_internal(Pos from, Pos to, bool enPassent = false, string new_history = "\0", bool real = true);
		inline bool will_get_check(Pos king, Pos from, Pos to);
		inline void next_turn() { turn *= -1; }


		// For input validation, must be zero based
		inline bool is_valid_pos(Pos pos) {
			const int x = pos.col - 'A';
			const int y = pos.row;
			if (x < 0 || x >= BOARD_SIZE || y < 0 || y > BOARD_SIZE)
				return false;
			return true;
		}

		inline bool will_remove_check(Pos king, Pos from, Pos to);

	public:
		// Current turn
		int turn = WHITE;

		// Stores all previous moves
		vector<Move> history;
		HistoryMode history_mode = HistoryMode::Positions;

		int current_checked_king = 0;

		int full_moves = 0;

		int halfmove_clock = 0;

		GameResult result = GameResult::StillPlaying;

		string get_fen();

		inline string get_history(string separator = " ", string separatorBetweenPoses = "=>")
		{
			string his = "";
			for (int i = 0; i < history.size(); i++)
			{
				if (history[i].notation != "\0")
					his += history[i].notation + separator;
				else
					his += history[i].from.to_string() + separatorBetweenPoses + history[i].to.to_string() + separator;
			}
			return his;
		}

		inline Piece& get_piece(Pos pos) {
			return board[pos.col - 'A'][pos.row];
		}

		string get_turn_name() {
			return (turn == WHITE ? "White" : "Black");
		}

		inline void import_board(Piece newBoard[BOARD_SIZE][BOARD_SIZE]) {
			memcpy(newBoard, board, sizeof(newBoard));
		}

		void initialize(GameMode mode);

		// Prints the board in the console
		string print_board();

		// Only for pawns
		inline void promote(int x, int y) {
			char promotedPawn;
			do {
				std::cout << "promte pawn to (r, n, b, q): "; std::cin >> promotedPawn;
				promotedPawn = tolower(promotedPawn);
			} while (promotedPawn != 'r' && promotedPawn != 'n' && promotedPawn != 'b' && promotedPawn != 'q');

			switch (promotedPawn) {
			case 'r':
				board[y][x] = y == 1 ? Piece(W_ROOK) : Piece(B_ROOK);
				break;
			case 'n':
				board[y][x] = y == 1 ? Piece(W_KNIGHT) : Piece(B_KNIGHT);
				break;
			case 'b':
				board[y][x] = y == 1 ? Piece(W_BISHOP) : Piece(B_BISHOP);
				break;
			case 'q':
				board[y][x] = y == 1 ? Piece(W_QUEEN) : Piece(B_QUEEN);
				break;
			}
		}

		bool can_rook_take(Pos rook, Pos piece)
		{
			if (((rook.col - 'A') == piece.col - 'A' && rook.row != piece.row) || (rook.row == piece.row && (rook.col - 'A') != (piece.col - 'A'))) {
				int x = (rook.col - 'A'), y = rook.row;
				x += (rook.col - 'A') < (piece.col - 'A') ? 1 : (rook.col - 'A') == (piece.col - 'A') ? 0 : -1;
				y += rook.row < piece.row ? 1 : rook.row == piece.row ? 0 : -1;
				while (x != (piece.col - 'A') || y != piece.row) {
					if (board[x][y].type != EMPTY)
						return false;
					x += (rook.col - 'A') < (piece.col - 'A') ? 1 : (rook.col - 'A') == (piece.col - 'A') ? 0 : -1;
					y += rook.row < piece.row ? 1 : rook.row == piece.row ? 0 : -1;
				}
				return true;
			}
			return false;
		}

		// Needs to be readable
		bool can_bishop_take(Pos bishop, Pos piece)
		{
			int from_y = bishop.row, from_x = bishop.col - 'A';
			int to_x = (piece.col - 'A');
			if (abs(from_x - to_x) == abs(from_y - piece.row)) {
				int x = from_x, y = from_y;
				x += from_x < piece.col - 'A' ? 1 : -1;
				y += from_y < piece.row ? 1 : -1;
				while (x != piece.col - 'A' && y != piece.row) {
					// If there is someone in between already
					if (board[x][y].type != EMPTY)
						return false;
					x += from_x < piece.col - 'A' ? 1 : -1;
					y += from_y < piece.row ? 1 : -1;
				}
				return true;
			}
			return false;
		}

		inline bool can_knight_take(Pos knight, Pos piece) {
			return
				(abs(knight.row - piece.row) == 2 && abs((knight.col - 'A') - (piece.col - 'A')) == 1) || // Horizontal
				(abs(knight.row - piece.row) == 1 && abs((knight.col - 'A') - (piece.col - 'A')) == 2);   // Vertical
		}

		// Not supports EnPassent
		inline bool can_pawn_take(Pos pawn, Pos piece) {
			Piece p = get_piece(pawn);
			int front = (p.color == WHITE ? pawn.row + 1 : pawn.row - 1);
			return (pawn.row == piece.row) && (piece.col == pawn.col - 1 || piece.col == pawn.col + 1);
		}

		// This method usually used with kings
		// It may be used in case of adding a manual engine
		bool is_checked(Pos pos)
		{
			pos.row--;
			const int to_x = pos.col - 'A', to_y = pos.row;
			const int eniemy = get_piece(pos).color * -1;
			for (int i = 0; i < BOARD_SIZE; i++) {
				for (int j = 0; j < BOARD_SIZE; j++) { 
					if (board[i][j].color == eniemy) {
						switch ((char)tolower(board[i][j].type))
						{
						case B_PAWN:
							if (can_pawn_take(Pos(i, j), pos))
								return true;
							break;
						case B_KNIGHT:
							if (can_knight_take(Pos(i, j), pos))
								return true;
							break;
						case B_ROOK:
							if (can_rook_take(Pos(i, j), pos))
								return true;
							break;
						case B_BISHOP:
							if (can_bishop_take(Pos(i, j), pos))
								return true;
							break;
						case B_QUEEN:
							if (can_bishop_take(Pos(i, j), pos) || can_rook_take(Pos(i, j), pos))
								return true;
							break;
						}
					}
				}
			}
			return false;
		}

		inline bool undo_last_move()
		{
			if (history.size() == 0)
				return false;
			// Here we may process the history, or save last board only
			move_piece_internal(history[history.size() - 1].to, history[history.size() - 1].from, false, "\0", false);
			history.pop_back(); // Removing the new one
			get_piece(history[history.size() - 1].to) = history[history.size() - 1].eaten;
			history.pop_back(); // Removing the last move
			return true;
		}

		// Moves
		Error move(Move move);
		Error move_piece(Pos from, Pos to);
		bool castle_queen_side();
		bool castle_king_side();
		bool can_castle_king_side(int color);
		bool can_castle_queen_side(int color);

		inline bool is_game_finished()
		{
			current_checked_king = 0;
			if (is_stalemate())
			{
				result = GameResult::Stalemate;
				return true;
			}
			if (halfmove_clock == 100)
			{
				result = GameResult::DrawBy50Move;
				return true;
			}
			if (is_draw_by_repeation())
			{
				result = GameResult::DrawByRepeatation;
				return true;
			}
			auto currentKing = (turn == WHITE ? whiteKing : blackKing);
			if (is_checked(currentKing))
			{
				current_checked_king = turn * -1;
				if (get_king_moves(currentKing).size() == 0)
				{
					// Checkmate!
					result = (GameResult)turn;
					return true;
				}
			}
			return false;
		}

		vector<Pos> get_king_moves(Pos king) {
			vector<Pos> moves;
			king.col--;
			int cnt = 3;
			while (cnt--)
			{
				if (!is_checked(king))
					moves.push_back(king);
				king.row--;
				if (!is_checked(king))
					moves.push_back(king);
				king.row+=2;
				if (!is_checked(king))
					moves.push_back(king);
				king.col++;
			}
			return moves;
		}

		string get_checked_king_name()
		{
			return (current_checked_king == WHITE ? "White" : "Black");
		}

		string get_game_result()
		{
			switch (result)
			{
			case StillPlaying:
				return "Still playing";
			case WhiteCheckmated:
				return "Checkmate! Black won";
			case Draw:
				return "Draw, both accepted!";
			case BlackCheckmated:
				return "Checkmate! Black won";
			case Stalemate:
				return "Stalemate, " + (string)(turn == WHITE ? "White" : "Black") + "doesn't have any legal move";
			case DrawByRepeatation:
				return "Draw by repetition";
			case DrawBy50Move:
				return "Draw by 50 move with no taking pieces or pawn advancing";
			default:
				return "Unknown";
			}
		}

		bool is_draw_by_repeation();
		bool is_stalemate();
	};
}