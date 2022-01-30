#pragma once
#include <string>
#include "Chess.h"

using std::string;
using namespace std;

namespace ConsoleChess {

namespace Engine {

string engine_app = "stockfish.exe";

string engine_execute(string command);
Move get_best_move(string fen, int depth = 22, int move_time = 1500);

}

}