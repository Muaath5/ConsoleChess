#include "Engine.h"

namespace ConsoleChess {

namespace Engine {

Engine::Engine(string app) : engine_app(app)
{
#if _WINDOWS
    if (!create_pipes())
        throw new exception("can't create pipes");
    create_processors();
#else
    cout << "Engines aren't supported in current OS" << endl;
    //create_engine_thread();
#endif
}
Engine::~Engine()
{
    //FreeConsole();
    // Closing handles
    CloseHandle(processInfo.hProcess);
    CloseHandle(processInfo.hThread);

    // Closing std handles
    CloseHandle(coutRead);
    CloseHandle(coutWrite);
    CloseHandle(cinRead);
    CloseHandle(cinWrite);
}

#ifdef _WINDOWS
bool Engine::create_pipes()
{
    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = FALSE;
    saAttr.lpSecurityDescriptor = NULL;

    if (!CreatePipe(&coutRead, &coutWrite, &saAttr, 0)) return false;

    // Ensure the read handle to the pipe for STDOUT is not inherited.

    if (!SetHandleInformation(coutRead, HANDLE_FLAG_INHERIT, 0)) return false;

    // Create a pipe for the child process's STDIN. 

    if (!CreatePipe(&cinRead, &cinWrite, &saAttr, 0)) return false;

    // Ensure the write handle to the pipe for STDIN is not inherited. 

    if (!SetHandleInformation(cinWrite, HANDLE_FLAG_INHERIT, 0)) return false;

    return true;
}

bool Engine::create_processors()
{
    ZeroMemory(&startupInfo, sizeof(STARTUPINFO));
    ZeroMemory(&processInfo, sizeof(PROCESS_INFORMATION));
    
    LPSTR app = (char*)engine_app.c_str();
    startupInfo.wShowWindow = TRUE;
    startupInfo.lpTitle = "UCI Integration with ConsoleChess";
    startupInfo.hStdOutput = coutWrite;
    startupInfo.hStdError = coutWrite;
    startupInfo.hStdInput = cinRead;
    startupInfo.dwFlags |= STARTF_USESTDHANDLES;
    startupInfo.cb = sizeof(startupInfo);

    // current directory
    const int MaxPath = 512;
    char currentDir[MaxPath];
    GetCurrentDirectory(MaxPath, currentDir);

    if (!CreateProcess(NULL,
        app,
        NULL,
        NULL,
        TRUE,
        0,
        NULL,
        currentDir,
        &startupInfo,
        &processInfo))
    {
        show_last_error("Engine::create_processors()");
        return false;
    }
    return true;
}

void Engine::show_last_error(string lpszFunction)
{
    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError();
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0, NULL);

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
        (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction.c_str()) + 40) * sizeof(TCHAR));
    StringCchPrintf((LPTSTR)lpDisplayBuf,
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"),
        lpszFunction, dw, lpMsgBuf);
    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK | MB_ICONERROR);
}
#endif


bool Engine::std_write(string command)
{
    DWORD writtenBytes;
    WriteFile(cinWrite, command.c_str(), command.size(), &writtenBytes, NULL);
    if (command.size() != writtenBytes)
        return false;
    return true;
}

string Engine::execute(string command)
{
    if (std_write(command))
    {
        int i = 12;
    }
    std::stringstream strStream ("");
    char lpBuffer[2] = { 0 };
    DWORD nBytesRead;
    while (true) {
        BOOL bResult = ReadFile(coutRead, lpBuffer, sizeof(char), &nBytesRead, NULL);
        if (bResult && nBytesRead) {
            strStream << lpBuffer;
        }
        else {
            break;
        }
    }
    return strStream.str();
}

Move Engine::get_best_move(string fen)
{
    execute("position fen " + fen);
    auto lines = str_split(execute("go depth 22"), '\n');
    string last_line = lines[lines.size() - 1];
    if (str_starts_with(last_line, "bestmove ") == false)
        throw new std::exception("No bestmove from output");
    string best_move = str_split(last_line, ' ')[1];
    Move move_obj(
        Pos(toupper(best_move[0]), best_move[1] - '0'),
        Pos(toupper(best_move[1]), best_move[2] - '0')
    );
    return move_obj;
}
bool Engine::is_ready()
{
    return (execute("isready") == "isreadyok");
}
void Engine::uci_new_game()
{
    execute("ucinewgame");
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

}

}