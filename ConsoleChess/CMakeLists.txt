# CMakeList.txt : CMake project for ConsoleChess, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (ConsoleChess "ConsoleChess.cpp" "ConsoleChess.h" "Chess.cpp" "Chess.h" "Piece.cpp" "Piece.h" "Constants.h" "ChessError.h" "Engine.cpp" "Engine.h")

# TODO: Add tests and install targets if needed.
