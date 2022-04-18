#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>

class Window
{
public:
    Window() : is_open(true), should_remove(false) { }
    virtual ~Window() { }

    virtual void renderImGui(class MainWindow& main) = 0;
    virtual const char* winname() = 0;

    constexpr void show()   { is_open = true; }
    constexpr void hide()   { is_open = false; }
    constexpr void remove() { is_open = false; should_remove = true; }

    constexpr bool isOpen() const { return is_open; }
protected:
    bool is_open, should_remove;
    friend class MainWindow;
    friend class MenuWindow;
};

class MainWindow
{
public:
    MainWindow(const char* name, size_t w, size_t h);
    ~MainWindow();

    constexpr std::list<std::unique_ptr<Window>>& windows()
    {
        return subwindows;
    }

    template<typename W>
    void apply_function(void (*function)(W& w))
    {
        for (auto& i : subwindows)
            if (dynamic_cast<W*>(&*i))
                function(*(W*)&*i);
    }

    template<typename W, typename... Args>
    W& addWindow(Args&&... args)
    {
        subwindows.push_back(std::make_unique<W>(args...));
        return *(W*)&*subwindows.back();
    }

    template<typename W>
    void removeWindow()
    {
        apply_function<W>([](W& w) { w.remove(); });
    }
    
    template<typename W>
    void showWindow()
    {
        apply_function<W>([](W& w) { w.show(); });
    }

    template<typename W>
    void hideWindow()
    {
        apply_function<W>([](W& w) { w.hide(); });
    }

    template<typename W>
    W& getWindow()
    {
        for (auto& i : subwindows)
            if (dynamic_cast<W*>(&*i))
                return *(W*)&*i;
        lg::fatal("Window not found");
        return *(W*)&*subwindows.front();
    }

    void renderImGui();
public:
    u32 color;
private:
    std::list<std::unique_ptr<Window>> subwindows;
    SDL_Window* sdl_context;
    SDL_GLContext gl_context;

    static bool init;
};

class MenuWindow : public Window
{
public:
    MenuWindow(Project& project, Serializer& serializer);
    virtual void renderImGui(MainWindow& main) override;
    virtual const char* winname() override { return "menu"; }
private:
    Project& project;
    Serializer& serializer;
};

class ColorWindow : public Window
{
public:
    ColorWindow();
    virtual void renderImGui(MainWindow& main) override;
    virtual const char* winname() override { return "color picker"; }
private:
    u32 cache[12];
    u32 current;
};

class SpritesWindow : public Window
{
public:
    SpritesWindow(Project& project);
    void updateProject(MainWindow& main);
    void addSprite(MainWindow& main, const std::string& name, u32 w, u32 h);
    void closeSprite(class CanvasWindow* canv);

    virtual void renderImGui(MainWindow& main) override;
    virtual const char* winname() override { return "sprite list"; }
private:
    Project& project;
    std::vector<std::tuple<std::string, Sprite*, bool, class CanvasWindow*>> sprites;
};

class NewSpriteWindow : public Window
{
public:
    NewSpriteWindow(SpritesWindow& sprites);

    virtual void renderImGui(MainWindow& main) override;
    virtual const char* winname() override { return "new sprite"; }
private:
    SpritesWindow& sprites;
    std::string name;
    int size[2];
};

class CanvasWindow : public Window
{
public:
    CanvasWindow(const std::string& name, Sprite& sprite);
    virtual ~CanvasWindow() override;
    
    void updateDraw(u32& color);
    void updateResizeState();
    void updateTexture();
    void resizeTexture();
    virtual void renderImGui(MainWindow& main) override;
    virtual const char* winname() override { return "canvas"; }
private:
    std::string name;
    Canvas canvas;
    u32 texture;
    float zoom;
    float playtime;
    bool play;

    enum {
        ResizeNone,
        ResizeNS,
        ResizeEW,
        ResizeAll
    };
    
    int resize;
};
