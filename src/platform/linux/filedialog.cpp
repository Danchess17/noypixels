#include <noypixel/noypixel.h>

#if NOY_PLATFORM == NOY_LINUX

std::optional<std::string> fileDialog(bool save)
{
    char name[1024];
    std::string format = "zenity --file-selection 2>/dev/null";
    
    if (save)
        format += " --save";

    FILE *f = popen(format.c_str(), "r");
    fgets(name, 1024, f);
    if (*name)
        *(name + strlen(name) - 1) = '\0';

    if (pclose(f))
        return {};
    return name;
}

#endif
