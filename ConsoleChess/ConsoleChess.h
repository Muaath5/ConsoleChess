// ConsoleChess.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <string>
#include "Chess.h"
#include "Constants.h"
#include "Engine.h"
#include "Pos.h"
#include "Move.h"

// End line
#define endl '\n'

// Reads line, may be empty
#define get_line(msg, x) std::cout << msg; if (std::getline(std::cin, x)) {}

// Asks question and enters input
#define ask(question, input) std::cout << question; std::cin >> input;
namespace ConsoleChess {

namespace UI {
	inline void clear_console();
	inline void initialize_board(Board &b, GameMode &mode, HistoryMode &historyMode);
	bool input_move(Board &board, Move &move);
}
inline string get_game_mode_name(GameMode mode);

}

using ConsoleChess::Move;
using ConsoleChess::Board;
using ConsoleChess::Pos;
using std::cout;
using std::cin;
using std::string;