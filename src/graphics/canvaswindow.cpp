#include <noypixel/noypixel.h>

CanvasWindow::CanvasWindow(const std::string& name, Sprite& sprite) : name(name), canvas(sprite, 0xffff00ff), texture(0), zoom(10.0f), frame(0)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, canvas.width(), canvas.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, canvas.getPixels());
}

CanvasWindow::~CanvasWindow()
{
    glDeleteTextures(1, &texture);
}

void CanvasWindow::renderImGui(MainWindow& main)
{
    ImGui::Begin(name.c_str(), &is_open);

    ImGui::DragFloat("Zoom", &zoom, 0.5f, 0.1f, 100.0f);
    ImGui::Text("size = %d x %d", canvas.width(), canvas.height());

    ImGui::Image((void*)(intptr_t)texture, ImVec2(canvas.width() * zoom, canvas.height() * zoom));
    
    if (ImGui::IsItemClicked())
    {
        ImVec2 mouse = ImGui::GetMousePos();
        mouse.x -= ImGui::GetItemRectMin().x;
        mouse.y -= ImGui::GetItemRectMin().y;
        mouse.x /= zoom;
        mouse.y /= zoom;
        canvas.setPixel(mouse.x, mouse.y, main.color);

        glBindTexture(GL_TEXTURE_2D, texture);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, canvas.width(), canvas.height(), GL_RGBA, GL_UNSIGNED_BYTE, canvas.getPixels());
    }

    // if (ImGui::Button("New Frame"))
    // {
    //     canvas.addFrame();
    //     frame = canvas.nrFrames() - 1;
    // }

    if (ImGui::Button("Save"))
        canvas.saveFrame();

    // if (ImGui::InputInt("Frame Number", &frame))
    // {
    //     if (frame >= canvas.nrFrames())
    //         frame = canvas.nrFrames() - 1;
    //     if (frame < 0)
    //         frame = 0;
    //     canvas.openFrame(frame);
    // }

    ImGui::End();

    if (!is_open)
        main.getWindow<SpritesWindow>().closeSprite(this);
}
