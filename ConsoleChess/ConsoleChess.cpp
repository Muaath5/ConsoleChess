// Copyright 2022
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
	if (modeStr == "both" || modeStr == "fenpositions" || modeStr == "fp" || modeStr == "pf" || modeStr == "all")
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

bool input_move(Board &board, Move &move)
{
	string notation;
	ask("Enter a move in format (E2E4): ", notation);
	if (notation == "U" || notation == "undo")
		board.undo_last_move();
	else
	{
		try {
			move = Move(notation);
		}
		catch (...)
		{
			return false;
		}
	}
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
HistoryMode historyMode = HistoryMode::Both;

int main()
{
	Board board;
	ConsoleChess::UI::initialize_board(board, mode, historyMode);
	while (!board.is_game_finished())
	{
		ConsoleChess::UI::clear_console();
		cout << board.print_board() << endl;
		cout << "Current turn is for " << board.get_turn_name() << endl;

		if (board.current_checked_king != 0)
		{
			cout << board.get_checked_king_name() << " is checked!" << endl;
		}

		Pos from, to;
		bool isErr = false;
		ConsoleChess::Error err = ConsoleChess::Error::Success;
		int depth = 10, movetime = 1500;
		//auto engine = ConsoleChess::Engine::Engine();
		//cout << engine.is_ready();
		// engine.uci();
		//engine.get_best_move();
		do
		{
			Move mv;
			if (isErr)
				cout << "ERROR: " << ConsoleChess::error_to_string(err) << endl;
			if (ConsoleChess::UI::input_move(board, mv) == false) break;
			err = board.move(mv);
			isErr = true;
		} while (err != ConsoleChess::Error::Success);
	}
	cout << board.get_game_result() << endl;
}
