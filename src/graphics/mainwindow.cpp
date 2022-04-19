#include <noypixel/noypixel.h>
#include <imgui/imgui.h>
#include <imgui/impl/imgui_impl_opengl3.h>
#include <imgui/impl/imgui_impl_sdl.h>

bool MainWindow::init = false;

MainWindow::MainWindow(const char* name, size_t w, size_t h)
{
    if (init)
        lg::fatal("MainWindow was already created");

    init = true;
    lg::trace("Setup SDL...");
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
        lg::fatal("SDL '{}'", SDL_GetError());

    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_MAXIMIZED);
    sdl_context = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, window_flags);
    gl_context = SDL_GL_CreateContext(sdl_context);
    SDL_GL_MakeCurrent(sdl_context, gl_context);
    SDL_GL_SetSwapInterval(1);
    lg::info("SDL {}.{}.{}", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL);

    lg::trace("Setup ImGui...");
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    lg::info("ImGui {}", IMGUI_VERSION);

    lg::trace("Setup OpenGL...");
    ImGui_ImplSDL2_InitForOpenGL(sdl_context, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);
    lg::info("GL {}", glGetString(GL_VERSION));

    ImGui::StyleColorsClassic();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding =                  0.0f;
    style.Colors[ImGuiCol_TitleBgActive] =  ImVec4(0.3f, 0.5f, 0.7f, 1.0f);
    style.Colors[ImGuiCol_MenuBarBg] =      ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
    style.Colors[ImGuiCol_TitleBg] =        ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
    style.Colors[ImGuiCol_WindowBg] =       ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
    style.Colors[ImGuiCol_Border] =         ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
    style.Colors[ImGuiCol_Header] =         ImVec4(0.2f, 0.2f, 0.2f, 1.0f);

    io.Fonts->AddFontFromFileTTF("dep/fonts/default.ttf", 16.0f);
}

MainWindow::~MainWindow()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(sdl_context);
    SDL_Quit();
}

void MainWindow::renderImGui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    for (auto it = subwindows.begin(); it != subwindows.end();)
    {
        if ((*it)->should_remove)
        {
            it = subwindows.erase(it);
            continue;
        }
        if ((*it)->is_open)
            (*it)->renderImGui(*this);
        ++it;
    }

    ImGui::Render();
    glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
        SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
    }
    SDL_GL_SwapWindow(sdl_context);
}
