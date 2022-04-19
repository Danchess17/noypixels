#include <noypixel/noypixel.h>

NewSpriteWindow::NewSpriteWindow(SpritesWindow& sprites) : sprites(sprites), name("unnamed"), size{ 16, 16 }
{ }

void NewSpriteWindow::renderImGui(MainWindow& main)
{
    ImGui::Begin("menuWindow newSpriteDialog", &is_open, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("New sprite:");
    ImGui::InputText("Enter name", name, 1024);
    ImGui::InputInt2("Enter sprite size", size);

    if (ImGui::Button("Ok"))
    {
        sprites.addSprite(main, name, size[0], size[1]);
        is_open = false;
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel"))
        is_open = false;
    ImGui::End();
}
