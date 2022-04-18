#pragma once

#define NOY_LINUX 1
#define NOY_WINDOWS 2

#if defined(__linux__)
#define NOY_PLATFORM NOY_LINUX
#elif defined(_WIN32)
#define NOY_PLATFORM NOY_WINDOWS
#else
#error Unknown platform
#endif

std::optional<std::string> fileDialog(bool save);
