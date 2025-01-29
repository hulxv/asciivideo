#include <stdio.h>
#include <time.h>
#include <stdint.h>
#ifdef _WIN32
#include <windows.h>

void clear_console()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD topLeft = {0, 0};
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD written;

    if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
        return;

    DWORD consoleSize = csbi.dwSize.X * csbi.dwSize.Y;

    // Fill the console with spaces
    FillConsoleOutputCharacter(hConsole, ' ', consoleSize, topLeft, &written);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, consoleSize, topLeft, &written);

    // Reset the cursor to the top-left corner
    SetConsoleCursorPosition(hConsole, topLeft);
}

void resize_terminal(int width, int height)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE)
    {
        fprintf(stderr, "Error: Cannot get console handle\n");
        return;
    }

    // Get the current console screen buffer info
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
    {
        fprintf(stderr, "Error: Cannot get console buffer info\n");
        return;
    }

    // Adjust the screen buffer size to be at least as large as the new window size
    COORD bufferSize;
    bufferSize.X = (SHORT)width;
    bufferSize.Y = (SHORT)height;

    if (bufferSize.X < csbi.srWindow.Right - csbi.srWindow.Left + 1)
    {
        bufferSize.X = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
    if (bufferSize.Y < csbi.srWindow.Bottom - csbi.srWindow.Top + 1)
    {
        bufferSize.Y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    }

    if (!SetConsoleScreenBufferSize(hConsole, bufferSize))
    {
        fprintf(stderr, "Error: Cannot set screen buffer size\n");
        return;
    }

    // Set the console window size
    SMALL_RECT windowSize = {0, 0, (SHORT)(width - 1), (SHORT)(height - 1)};
    if (!SetConsoleWindowInfo(hConsole, TRUE, &windowSize))
    {
        fprintf(stderr, "Error: Cannot set console window size\n");
        return;
    }
    clear_console();
}

#else
#define __USE_POSIX
#include <sys/ioctl.h>
#include <unistd.h>
#include <signal.h>
void resize_terminal(int width, int height)
{
    struct winsize ws;

    // Set the new window size
    ws.ws_row = height;
    ws.ws_col = width;

    // Apply the new window size using ioctl
    if (ioctl(STDOUT_FILENO, TIOCSWINSZ, &ws) == -1)
    {
        perror("Error resizing terminal");
        return;
    }

    // Notify the terminal about the size change
    kill(getpid(), SIGWINCH);

    printf("Terminal resized to %dx%d\n", width, height);
}

#endif

uint64_t get_current_time_us()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000000 + ts.tv_nsec / 1000;
}