# CMakeList.txt : CMake project for ConsoleChess, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set (ENGINE_SOURCE "Engine.cpp" "Engine.h")
set (ENGINE_APP_NAME "stockfish.exe")
set (CONSOLE_CHESS_SOURCE "Engine.cpp" "Engine.h" "ConsoleChess.cpp" "ConsoleChess.h" "Chess.cpp" "Chess.h" "Piece.cpp" "Piece.h" "Constants.h" "ChessError.h" )

# Add source to this project's executable.
add_executable (ConsoleChess "Engine.cpp" "Engine.h" "ConsoleChess.cpp" "ConsoleChess.h" "Chess.cpp" "Chess.h" "Piece.cpp" "Piece.h" "Constants.h" "ChessError.h" "Pos.h" "Pos.cpp" "Move.h" "ChessAudio.h" "ChessAudio.cpp")

# Adding engine uci.exe
message ("${ENGINE_APP_NAME} will be included in output folder")
file (COPY ${ENGINE_APP_NAME} DESTINATION ${CMAKE_BINARY_DIR})

# TODO: Add tests and install targets if needed.
