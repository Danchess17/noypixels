objdir "bin/obj/%{cfg.buildcfg}/%{prj.name}"
targetdir "bin/%{cfg.buildcfg}"
includedirs { "include", "dep/windows/include" }
libdirs { "dep/windows/lib" }

filter "configurations:Debug"
    symbols "On"
    defines { "NOY_DEBUG" }
    buildoptions { "-fsanitize=undefined,address" }
    linkoptions { "-fsanitize=undefined,address" }

filter "configurations:Release"
    optimize "On"

solution "noy"
    configurations { "Debug", "Release" }

project "noypixels"
    kind "ConsoleApp"
    language "C++"
    files { "src/**.cpp" }
    defines { "SDL_MAIN_HANDLED" }
    links { "stb", "imgui", "SDL2", "SDL2main", "yaml-cpp" , "opengl32", "glu32", "Gdi32", "winmm", "cfgmgr32", "Ole32", "Imm32", "OleAut32", "Version", "Setupapi", "Comdlg32" }

project "stb"
    kind "StaticLib"
    language "C++"
    files { "vendor/stb/**.cpp" }

project "imgui"
    kind "StaticLib"
    language "C++"
    files { "vendor/imgui/**.cpp" }
    includedirs { "include/imgui", "include/imgui/impl" }
