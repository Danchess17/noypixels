#pragma once

#include <iostream>

namespace lg {

    enum class Level
    {
        Trace, Info, Warn, Error, Fatal, None
    };

    void level(Level level);
    Level level();

    const char* log_color(Level level);
    const char* log_name(Level level);

    template<typename T, typename... Args>
    void log_next(const char*& fmt, const T& t, const Args&... args)
    {
        while(*fmt != '{') {
            if (*fmt == '\\')
                fmt++;
            std::cerr << *fmt++;
        }
        while(*fmt++ != '}');
        std::cerr << t;
        if constexpr (sizeof...(args))
            log_next(fmt, args...);
    }

    template<typename... Args>
    void log(Level lev, const char* fmt, const Args&... args)
    {
        if (lev < level())
            return;

        if (lev > Level::Trace)
            std::cerr << log_color(lev) << "noy: " << log_name(lev) << ": ";

        if constexpr (sizeof...(args))
            log_next(fmt, args...);
        std::cerr << fmt << log_color(Level::None) << '\n';
    }

    template<typename... Args>
    void trace(const char* fmt, const Args&... args)
    {
        log(Level::Trace, fmt, args...);
    }

    template<typename... Args>
    void info(const char* fmt, const Args&... args)
    {
        log(Level::Info, fmt, args...);
    }

    template<typename... Args>
    void warn(const char* fmt, const Args&... args)
    {
        log(Level::Warn, fmt, args...);
    }

    template<typename... Args>
    void error(const char* fmt, const Args&... args)
    {
        log(Level::Error, fmt, args...);
    }

    template<typename... Args>
    void fatal(const char* fmt, const Args&... args)
    {
        log(Level::Fatal, fmt, args...);
        exit(1);
    }
}
