#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#include <Windows.h>
#include <stdexcept>

static void enableVirtualConsole() {
    // Set output mode to handle virtual terminal sequences
    HANDLE outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    if(outputHandle == INVALID_HANDLE_VALUE) {
        throw std::runtime_error("Invalid handle value!");
    }

    DWORD consoleMode = 0;
    if(!GetConsoleMode(outputHandle, &consoleMode)) {
        throw std::runtime_error("Unable to get current console mode!");
    }

    consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if(!SetConsoleMode(outputHandle, consoleMode)) {
        throw std::runtime_error("Unable to set console mode!");
    }
}
#else
static void enableVirtualConsole() {
    // Necessary functionality exists by default, do nothing
}
#endif