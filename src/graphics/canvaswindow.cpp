#include <noypixel/noypixel.h>

CanvasWindow::CanvasWindow(const std::string& name, Sprite& sprite) : name(name), canvas(sprite, 0xffff00ff),
    texture(0), zoom(10.0f), playtime(0.0f), play(false), resize(0)
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

void CanvasWindow::updateDraw(u32& color)
{
    if (resize) return;

    if (!ImGui::IsItemHovered(ImGuiHoveredFlags_None))
        return;
    
    ImVec2 mouse = ImGui::GetMousePos();
    mouse.x -= ImGui::GetItemRectMin().x;
    mouse.y -= ImGui::GetItemRectMin().y;

    mouse.x /= zoom;
    mouse.y /= zoom;

    if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
    {
        canvas.setPixel(mouse.x, mouse.y, color);
        updateTexture();
    }

    if (ImGui::IsMouseDown(ImGuiMouseButton_Right))
        color = canvas.getPixel(mouse.x, mouse.y);
}

void CanvasWindow::updateResizeState()
{
    if (!resize && !ImGui::IsItemHovered(ImGuiHoveredFlags_None))
        return;
    
    ImVec2 mouse = ImGui::GetMousePos(), end = ImGui::GetItemRectMax();
    mouse.x -= end.x;
    mouse.y -= end.y;

    int tmp_resize = ResizeNone;

    if (mouse.x >= -10 && mouse.x < 0)
        tmp_resize |= ResizeEW;
    if (mouse.y >= -10 && mouse.y < 0)
        tmp_resize |= ResizeNS;

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) || ImGui::IsMouseReleased(ImGuiMouseButton_Left))
        resize = tmp_resize;

    if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
    {
        if (resize)
        {
            int w = canvas.width(), h = canvas.height();
            if (resize & ResizeEW)
                w += (int)(mouse.x / zoom);
            if (resize & ResizeNS)
                h += (int)(mouse.y / zoom);
            if (((u32)w != canvas.width() || (u32)h != canvas.height()) && w > 0 && h > 0)
            {
                canvas.resize(w, h);
                resizeTexture();
            }
        }
        return;
    }

    if (!tmp_resize)
        ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
    else
        ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS + tmp_resize - 1);
}

void CanvasWindow::updateTexture()
{
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, canvas.width(), canvas.height(), GL_RGBA, GL_UNSIGNED_BYTE, canvas.getPixels());
}

void CanvasWindow::resizeTexture()
{
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, canvas.width(), canvas.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, canvas.getPixels());
}

void CanvasWindow::renderImGui(MainWindow& main)
{
    ImGui::Begin(name.c_str(), &is_open);
    ImGui::DragFloat("Zoom", &zoom, 0.5f, 0.1f, 100.0f);
    ImGui::Text("size = %d x %d", canvas.width(), canvas.height());

    ImGui::Text("playtime = %f", playtime);

    int nr = 0;
    float len, sum = 0.0f;
    for (auto& i : canvas.getFrames())
        sum += i.time;

    while (playtime >= sum)
        playtime -= sum;

    if (play)
    {
        canvas.openFrame((f32)playtime);
        updateTexture();
        playtime += 0.01f;
    }
    
    ImVec2 cursor = ImGui::GetCursorPos();
    ImGui::ImageButton((void*)(intptr_t)texture, ImVec2(canvas.width() * zoom, canvas.height() * zoom), ImVec2(0, 0), ImVec2(1, 1), 0);
    updateDraw(main.color);
    ImGui::SetCursorPos(cursor);
    ImGui::InvisibleButton("invisible", ImVec2(canvas.width() * zoom + 10, canvas.height() * zoom + 10));
    updateResizeState();

    if (ImGui::Button("New Frame", ImVec2(100, 25)))
    {
        canvas.addFrame();
        updateTexture();
    }

    if (ImGui::Button("Save", ImVec2(100, 25)))
        canvas.save();

    len = ImGui::GetWindowSize().x * 0.95f;

    for (auto& i : canvas.getFrames())
    {
        if (nr)
        {
            ImGui::SameLine();
            ImGui::SetCursorPosX(cursor.x + 2.0f);
        }

        ImGui::PushItemWidth(i.time * len / sum);
        ImGui::PushID((size_t)this + nr);

        cursor.x = ImGui::GetCursorPosX() + i.time * len / sum;
        ImGui::DragFloat("", (float*)&i.time, 0.05f, 0.05f, 10.0f);
        ImGui::PopID();
        
        if (ImGui::IsItemClicked())
        {
            canvas.openFrame((u32)nr);
            playtime = canvas.frameTime();
            updateTexture();
        }
        nr++;
    }

    ImGui::Checkbox("play", &play);

    ImGui::End();

    if (!is_open)
        main.getWindow<SpritesWindow>().closeSprite(this);
}
