#include "Engine.h"

namespace ConsoleChess {

namespace Engine {

string engine_execute(string command)
{
    command = engine_app + " " + command;
    char buffer[128];
    string result = "";
    FILE* pipe = _popen(command.c_str(), "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != NULL)
                result += buffer;
        }
    }
    catch (...) {
        _pclose(pipe);
        throw;
    }
    _pclose(pipe);
    return result;
}

std::vector<string> str_split(string str, char seperator)
{
    std::vector<string> res;
    int currIndex = 0, i = 0;
    int startIndex = 0, endIndex = 0;
    while (i <= str.size())
    {
        if (str[i] == seperator || i == str.size())
        {
            endIndex = i;
            string subStr = "";
            subStr.append(str, startIndex, endIndex - startIndex);
            res.push_back(subStr);
            currIndex += 1;
            startIndex = endIndex + 1;
        }
        i++;
    }
    return res;
}

inline bool str_starts_with(string str, string first)
{
    return (str.substr(0, first.size()) == first);
}

Move get_best_move(string fen, int depth, int move_time)
{
    engine_execute("position fen " + fen);
    string cmd = "go ";
    string depthStr = "depth ";
    string moveTimeStr = " movetime ";
    string str = engine_execute(cmd + std::to_string(depth) + moveTimeStr + std::to_string(move_time));
    auto lines = str_split(str, '\n');
    string last_line = lines[lines.size() - 1];
    if (str_starts_with(last_line, "bestmove "))
    {
        string best_move = str_split(last_line, ' ')[1];
        Move move_obj(
            Pos(toupper(best_move[0]), best_move[1]-'0'),
            Pos(toupper(best_move[1]), best_move[2]-'0')
        );
        return move_obj;
    }
    else
    {
        throw new std::exception("Engine didn't send bestmove result");
    }
}

}

}