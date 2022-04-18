#include <noypixel/noypixel.h>


ColorWindow::ColorWindow()
{
    for (u32 i = 0; i < 12; i++)
        cache[i] = 0xff000000;
    current = 0;
}

void ColorWindow::renderImGui(MainWindow& main)
{
    ImVec4 col = ImGui::ColorConvertU32ToFloat4(main.color);
    ImGui::Begin("Color", &is_open);
    ImGui::ColorPicker3("", (float*)&col);

    cache[current] = main.color = ImGui::ColorConvertFloat4ToU32(col) | 0xff000000;

    for (u32 i = 0; i < 12; i++)
    {
        if (i % 6)
            ImGui::SameLine();
        if (ImGui::ColorButton(("cache color " + std::to_string(i)).c_str(), ImGui::ColorConvertU32ToFloat4(cache[i])))
        {
            current = i;
            main.color = cache[current];
        }
    }

    ImGui::End();
}
