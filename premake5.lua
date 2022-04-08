objdir "bin/obj/%{cfg.buildcfg}/%{prj.name}"
targetdir "bin/%{cfg.buildcfg}"
includedirs { "include" }

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
    links { "stb", "imgui", "GL", "SDL2", "yaml-cpp" }

project "stb"
    kind "StaticLib"
    language "C++"
    files { "vendor/stb/**.cpp" }

project "imgui"
    kind "StaticLib"
    language "C++"
    files { "vendor/imgui/**.cpp" }
    includedirs { "include/imgui", "include/imgui/impl" }
