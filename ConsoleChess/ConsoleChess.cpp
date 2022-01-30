﻿// Copyright 2022
// Written by: mdubaisi, Muaath_5
// A C++ console app to play chess with a friend offline

#include "ConsoleChess.h"

namespace ConsoleChess {

namespace UI {

inline void initialize_board(Board &b, GameMode &mode, HistoryMode &historyMode)
{
	string modeStr;

	get_line("Enter game mode: ", modeStr);
	if (modeStr == "960" || modeStr == "random" || modeStr == "r" || modeStr == "rand")
		mode = GameMode::Mode960;

	get_line("Enter history mode: ", modeStr);
	if (modeStr == "import")
	{
		string historyImported;
		get_line("You'll import a game, input the history of moves with a space between each: ", historyImported);
		//pair<Error, vector<Move>> err = ConsoleChess::parse_history(historyImported);
		//if (err.first == Error::Success)
		//    board.import_game(err.second);
	}
	if (modeStr == "disable" || modeStr == "no" || modeStr == "d" || modeStr == "n")
		historyMode = HistoryMode::Disable;
	if (modeStr == "both" || modeStr == "fenpositions" || modeStr == "fp" || modeStr == "pf")
		historyMode = HistoryMode::Both;
	if (modeStr == "fen" || modeStr == "f" || modeStr == "FEN")
		historyMode = HistoryMode::FEN;

	b.initialize(mode);
	b.history_mode = historyMode;
	cout << "Game mode is " << get_game_mode_name(mode) << endl;
}


inline void clear_console()
{
#ifdef _WIN32
	std::system("cls");
#else
	std::system("clear");
#endif
}

// will return false in castling only
bool input_move(Board &board, Pos &from, Pos &to)
{
InputError:
	ask("Enter a move in format (E2E4): ", from.col);
	string castle;
	// If castling
	if (from.col == 'O')
	{
		cin >> castle;
		if (castle == "-O") {
			if (!board.king_side_castle()) {
				cout << "Castling in king's side isn't allowed!" << endl;
				goto InputError;
			}
		}
		else if (castle == "-O-O") {
			if (!board.queen_side_castle()) {
				cout << "Castling in queen's side isn't allowed!" << endl;
				goto InputError;
			}
		}
		else {
			cout << "Castling must be in format O-O or O-O-O" << endl;
			goto InputError;
		}
		return false;
	}
	else if (from.col == 'U')
	{
		board.undo_last_move();
		ConsoleChess::UI::clear_console();
		cout << "Undone last move!" << endl;
		return false;
	}
	cin >> from.row >> to.col >> to.row;
	from.col = toupper(from.col), to.col = toupper(to.col);
	return true;
}

}

inline string get_game_mode_name(GameMode mode)
{
	switch (mode)
	{
	case Normal:
		return "default";
	case Mode960:
		return "960";
	default:
		return "unknown";
	}
}


}

GameMode mode = GameMode::Normal;
HistoryMode historyMode = HistoryMode::Positions;

int main()
{
	Board board;
	ConsoleChess::UI::initialize_board(board, mode, historyMode);
	while (!board.checkmated())
	{
		ConsoleChess::UI::clear_console();
		cout << board.print_board() << endl;
		cout << "Current turn is for " << board.get_turn_name() << endl;

		Pos from, to;
		bool isErr = false;
		ConsoleChess::Error err = ConsoleChess::Error::Success;
		do
		{
			if (isErr)
				cout << "ERROR: " << ConsoleChess::error_to_string(err) << endl;
			if (ConsoleChess::UI::input_move(board, from, to) == false) break;
			err = board.move_piece(from, to);
			isErr = true;
		} while (err != ConsoleChess::Error::Success);
	}
	cout << board.get_turn_name() << " won!" << endl;
}
