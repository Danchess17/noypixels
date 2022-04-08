#include <noypixel/noypixel.h>

MenuWindow::MenuWindow(Project& project, Serializer& serializer) : project(project), serializer(serializer)
{ }

void MenuWindow::renderImGui(MainWindow& main)
{
    char name[1024];

    if(ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New Project", "Ctrl+Shift+N"))
            {
                serializer.deserialize(nullptr);
                main.getWindow<SpritesWindow>().updateProject(main);
            }
            if (ImGui::MenuItem("Load Project", "Ctrl+Shift+O"))
            {
                FILE *f = popen("zenity --file-selection 2>/dev/null", "r");
                fgets(name, 1024, f);
                if (*name)
                    *(name + strlen(name) - 1) = '\0';
                if (!pclose(f))
                {
                    serializer.deserialize(name);
                    main.getWindow<SpritesWindow>().updateProject(main);
                }
            }
            if (ImGui::MenuItem("Save Project", "Ctrl+Shift+S"))
                serializer.serialize("a.png", "a.json");

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
