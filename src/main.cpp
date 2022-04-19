#include <imgui/impl/imgui_impl_sdl.h>
#include <noypixel/noypixel.h>

int main(int, char**)
{
    lg::level(lg::Level::Trace);

    Project proj;
    Serializer s(proj);

    MainWindow window("noypixels", 800, 600);

    window.addWindow<ColorWindow>();
    window.addWindow<MenuWindow>(proj, s);
    window.addWindow<SpritesWindow>(proj);
    window.addWindow<NewSpriteWindow>(window.getWindow<SpritesWindow>()).hide();

    bool done = false;
    while (!done)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
        }
        window.renderImGui();
    }
    return 0;
}
