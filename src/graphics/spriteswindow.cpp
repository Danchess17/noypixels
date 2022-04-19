#include <noypixel/noypixel.h>

SpritesWindow::SpritesWindow(Project& project) : project(project)
{
    for (auto&[name, sprite] : project)
        sprites.emplace_back(name, &sprite, false, nullptr);
}

void SpritesWindow::updateProject(MainWindow& main)
{
    main.removeWindow<CanvasWindow>();
    sprites.clear();
    for (auto&[name, sprite] : project)
        sprites.emplace_back(name, &sprite, false, nullptr);
}

void SpritesWindow::addSprite(MainWindow& main, const char* name, u32 w, u32 h)
{
    Sprite& s = project.newSprite(name, w, h);
    sprites.emplace_back(name, &s, true, &main.addWindow<CanvasWindow>(name, s));
}

void SpritesWindow::closeSprite(class CanvasWindow* canv)
{
    for (auto&[name, sprite, open, window] : sprites)
        if (window == canv)
        {
            window->remove();
            window = nullptr;
            open = false;
        }
}

void SpritesWindow::renderImGui(MainWindow& main)
{
    ImGui::Begin("Sprites", &is_open);
    
    for (auto&[name, sprite, open, window] : sprites)
    {
        if (ImGui::Checkbox(name.c_str(), &open))
            if (open)
                window = &main.addWindow<CanvasWindow>(name, *sprite);
            else
            {
                window->remove();
                window = nullptr;
            }
    }

    if (ImGui::Button("add"))
        main.showWindow<NewSpriteWindow>();

    ImGui::End();
}
