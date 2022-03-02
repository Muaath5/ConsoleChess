#include <Constants.h>
#include <string>
#ifdef WIN32
#include <Windows.h>
#endif

using std::string;

#define CASTLING_SOUND_WAV "castling.wav"
#define EATING_SOUND_WAV "eating.wav"
#define MOVE_SOUND_WAV "move.wav"


namespace ConsoleChess {
	void play_sound(string file_name);
}