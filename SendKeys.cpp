/*
SendKeys.cpp - Simulate keyboard input on Windows

Build Instructions:
1. Open "x64 Native Tools Command Prompt for VS".
2. Navigate to the folder containing SendKeys.cpp.
3. Run:
   cl.exe SendKeys.cpp /Fe:SendKeys.exe

Usage:
  SendKeys.exe --text <text> [--new-line] [--wait <seconds>]

Options:
  --text <text>      The text to type.
  --new-line        Send an Enter key after typing.
  --wait <seconds>  Wait before typing (default: 3 seconds).
*/

#include <windows.h>
#include <string>
#include <thread>
#include <iostream>

#pragma comment(lib, "User32.lib")
#pragma warning(disable : 4530) // Disable C++ exception handler warning

#define DEFAULT_WAIT_SECONDS 2

void SendString(const std::string &text)
{
    for (char c : text)
    {
        INPUT input = {};
        input.type = INPUT_KEYBOARD;
        input.ki.wVk = 0;
        input.ki.wScan = c;
        input.ki.dwFlags = KEYEVENTF_UNICODE;
        SendInput(1, &input, sizeof(INPUT));

        // Key up event
        input.ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
        SendInput(1, &input, sizeof(INPUT));

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void SendEnter()
{
    INPUT input = {};
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = VK_RETURN;
    SendInput(1, &input, sizeof(INPUT));
    input.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &input, sizeof(INPUT));
}

void PrintHelp()
{
    std::cout << "Usage: SendKeys.exe --text <text> [--new-line] [--wait <seconds>]" << std::endl;
    std::cout << std::endl
              << "Options:" << std::endl;
    std::cout << "  --text <text>     The text to type." << std::endl;
    std::cout << "  --new-line        Send an Enter key after typing." << std::endl;
    std::cout << "  --wait <seconds>  Wait before typing (default: " << DEFAULT_WAIT_SECONDS << " seconds)." << std::endl;
    std::cout << "  --help            Show this help message." << std::endl
              << std::endl;
}

int main(int argc, char *argv[])
{
    std::string text;
    bool sendNewLine = false;
    int waitSeconds = DEFAULT_WAIT_SECONDS;

    // Parse arguments
    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "--text" && i + 1 < argc)
        {
            text = argv[++i];
        }
        else if (arg == "--new-line")
        {
            sendNewLine = true;
        }
        else if (arg == "--wait" && i + 1 < argc)
        {
            waitSeconds = atoi(argv[++i]);
        }
        else if (arg == "--help")
        {
            PrintHelp();
            return 0;
        }
    }

    if (text.empty())
    {
        PrintHelp();
        return 1;
    }

    std::cout << "Waiting " << waitSeconds << " second(s) before typing..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(waitSeconds));

    SendString(text);
    if (sendNewLine)
    {
        SendEnter();
    }
    return 0;
}
