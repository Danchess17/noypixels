#include <noypixel/noypixel.h>

MenuWindow::MenuWindow(Project& project, Serializer& serializer) : project(project), serializer(serializer)
{ }

void MenuWindow::renderImGui(MainWindow& main)
{
    if(ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New Project", "Ctrl+Shift+N"))
            {
                serializer.deserialize("");
                main.getWindow<SpritesWindow>().updateProject(main);
            }
            if (ImGui::MenuItem("Load Project", "Ctrl+Shift+O"))
            {
                auto name = fileDialog(false);
                if (name)
                {
                    serializer.deserialize(name.value().c_str());
                    main.getWindow<SpritesWindow>().updateProject(main);
                }
            }
            if (ImGui::MenuItem("Save Project", "Ctrl+Shift+S"))
            {
                auto name = fileDialog(true);
                if (name)
                    serializer.serialize(name.value());
            }

            ImGui::Separator();
            
            if (ImGui::MenuItem("New Sprite", "Ctrl+N"))
                main.showWindow<NewSpriteWindow>();
            
            ImGui::Separator();

            if (ImGui::MenuItem("Exit"))
                lg::fatal("user exit");
            
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Windows"))
        {
            for (auto& i : main.windows())
                if (ImGui::MenuItem(i->winname(), "", &(i->is_open)));
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}
