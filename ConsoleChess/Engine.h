#pragma once

#include <string>
#include "Move.h"
#include <vector>
#if _WINDOWS
#include <Windows.h>
#include <sstream>
#include <strsafe.h>
#include <iostream>
#endif


using std::string;
using std::exception;

namespace ConsoleChess {

namespace Engine {

    // An engine that uses UCI
    struct Engine {
    private:
#ifdef _WINDOWS
        HANDLE cinRead, cinWrite, coutRead, coutWrite;
        STARTUPINFO startupInfo;
        bool create_pipes();
        bool create_processors();
        void show_last_error(string lpszFunction);
#endif

    public: 
        string engine_app = "./stockfish.exe";
        Engine(string app = "./stockfish.exe");
        ~Engine();

    protected:
        string workingDir;
        bool std_write(string command);
        //string std_read();
        string execute(string command);
        PROCESS_INFORMATION processInfo;

    public:
        Move get_best_move(string fen);
        bool is_ready();
        void uci_new_game();
    };


    std::vector<string> str_split(string str, char split);
    inline bool str_starts_with(string str, string first);
}
}