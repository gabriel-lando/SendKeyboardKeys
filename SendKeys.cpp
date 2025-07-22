/*
SendKeys.cpp - Simulate keyboard input on Windows

Build Instructions:
1. Open "x64 Native Tools Command Prompt for VS".
2. Navigate to the folder containing SendKeys.cpp.
3. Run:
   cl.exe SendKeys.cpp /Fe:SendKeys.exe

Usage:
  SendKeys.exe --text <text> [--new-line] [--wait <seconds> | --wait-scroll-lock] [--keep]

Options:
  --text <text>        The text to type.
  --new-line           Send an Enter key after typing.
  --wait <seconds>     Wait before typing (default: 2 seconds).
  --wait-scroll-lock   Wait for Scroll Lock toggle before typing (overrides --wait).
  --keep               Keep repeating until Ctrl+C is pressed.
  --help               Show this help message.

Examples:
  SendKeys.exe --text "Hello" --wait 5
  SendKeys.exe --text "World" --new-line --wait-scroll-lock
  SendKeys.exe --text "Test" --keep --wait 3
*/

#include <windows.h>
#include <string>
#include <thread>
#include <iostream>
#include <csignal>

#pragma comment(lib, "User32.lib")
#pragma warning(disable : 4530) // Disable C++ exception handler warning

#define DEFAULT_WAIT_SECONDS 2

// Global flag for Ctrl+C handling
volatile bool keepRunning = true;

void SignalHandler(int signal)
{
    if (signal == SIGINT)
    {
        std::cout << "\nCtrl+C pressed. Exiting..." << std::endl;
        keepRunning = false;
    }
}

void WaitForScrollLockToggle()
{
    bool initialState = (GetKeyState(VK_SCROLL) & 0x0001) != 0;
    bool hasToggled = false;

    std::cout << "Scroll Lock is " << (initialState ? "ON" : "OFF") << std::endl;

    if (initialState)
    {
        std::cout << "Turn OFF Scroll Lock to begin..." << std::endl;
    }
    else
    {
        std::cout << "Turn ON Scroll Lock, then OFF to begin..." << std::endl;
    }

    while (true)
    {
        Sleep(100);
        bool currentState = (GetKeyState(VK_SCROLL) & 0x0001) != 0;

        // If initially on, just wait for off
        if (initialState && !currentState)
        {
            std::cout << "Starting..." << std::endl;
            break;
        }

        // If initially off, wait for on->off sequence
        if (!initialState)
        {
            if (currentState && !hasToggled)
            {
                hasToggled = true;
                std::cout << "Now turn OFF Scroll Lock..." << std::endl;
            }
            else if (!currentState && hasToggled)
            {
                std::cout << "Starting..." << std::endl;
                break;
            }
        }

        // Check for Ctrl+C during scroll lock wait
        if (!keepRunning)
        {
            return;
        }
    }
}

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

        Sleep(50);
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
    std::cout << "Usage: SendKeys.exe --text <text> [--new-line] [--wait <seconds> | --wait-scroll-lock] [--keep]" << std::endl;
    std::cout << std::endl
              << "Options:" << std::endl;
    std::cout << "  --text <text>        The text to type." << std::endl;
    std::cout << "  --new-line           Send an Enter key after typing." << std::endl;
    std::cout << "  --wait <seconds>     Wait before typing (default: " << DEFAULT_WAIT_SECONDS << " seconds)." << std::endl;
    std::cout << "  --wait-scroll-lock   Wait for Scroll Lock toggle before typing (overrides --wait)." << std::endl;
    std::cout << "  --keep               Keep repeating until Ctrl+C is pressed." << std::endl;
    std::cout << "  --help               Show this help message." << std::endl
              << std::endl;
}

int main(int argc, char *argv[])
{
    std::string text;
    bool sendNewLine = false;
    int waitSeconds = DEFAULT_WAIT_SECONDS;
    bool waitForScrollLock = false;
    bool keepRepeating = false;

    // Set up Ctrl+C handler
    signal(SIGINT, SignalHandler);

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
        else if (arg == "--wait-scroll-lock")
        {
            waitForScrollLock = true;
        }
        else if (arg == "--keep")
        {
            keepRepeating = true;
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

    if (keepRepeating)
    {
        std::cout << "Repeating mode enabled. Press Ctrl+C to stop." << std::endl;
    }

    do
    {
        if (waitForScrollLock)
        {
            WaitForScrollLockToggle();
        }
        else
        {
            std::cout << "Waiting " << waitSeconds << " second(s) before typing..." << std::endl;
            Sleep(waitSeconds * 1000);
        }

        // Check if we should continue before typing
        if (!keepRunning)
        {
            break;
        }

        SendString(text);
        if (sendNewLine)
        {
            SendEnter();
        }

        // If repeating, add a small delay before next iteration
        if (keepRepeating && keepRunning)
        {
            Sleep(500);
        }

    } while (keepRepeating && keepRunning);

    return 0;
}
