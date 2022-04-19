#include <noypixel/log.h>

namespace lg {

    static const char* log_names[] = {
        "", "info", "warning", "error", "fatal error", ""
    };

    static const char* log_colors[] = {
        "", "\033[32m", "\033[33m", "\033[31m", "\033[31;1m", "\033[0m"
    };

    Level current_level;

    const char* log_color(Level level)
    {
        return log_colors[(int)level];
    }

    const char* log_name(Level level)
    {
        return log_names[(int)level];
    }
    
    void level(Level level)
    {
        current_level = level;
    }

    Level level()
    {
        return current_level;
    }    
}