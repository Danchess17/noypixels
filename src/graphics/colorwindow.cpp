#include <noypixel/noypixel.h>

void ColorWindow::renderImGui(MainWindow& main)
{
    ImVec4 col = ImGui::ColorConvertU32ToFloat4(main.color);
    ImGui::Begin("Color", &is_open);
    ImGui::ColorPicker3("", (float*)&col);
    ImGui::End();
    main.color = ImGui::ColorConvertFloat4ToU32(col) | 0xff000000;
}
