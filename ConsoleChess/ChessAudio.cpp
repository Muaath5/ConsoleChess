#include <ChessAudio.h>

namespace ConsoleChess {
	void play_sound(string file_name)
	{
#if WIN32
		if (!PlaySound(file_name.c_str(), NULL, SND_FILENAME | SND_ASYNC))
			throw new exception(GetLastError());
#endif
	}
}