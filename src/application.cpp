#include "application.hpp"

#include <stdexcept>

Application::Application(const Rect& rect, const std::string& title)
    : rect(rect), title(title), handle(nullptr), tab(menuTabs::File)
{
    init();
}

Application::~Application() {
    destroy();
}

void Application::run() {
    while (!glfwWindowShouldClose(handle)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        menu();

        float menuHeight = ImGui::GetFrameHeight();

        ImGuiIO& io = ImGui::GetIO();

        ImGui::SetNextWindowPos(ImVec2(0, menuHeight));
        ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y - menuHeight));

        ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        ImGui::Begin(title.c_str(), nullptr, flags);
        render();
        ImGui::End();

        ImGui::PopStyleVar();

        ImGui::Render();

        int displayWidth;
        int displayHeight;
        glfwGetFramebufferSize(handle, &displayWidth, &displayHeight);

        glViewport(0, 0, displayWidth, displayHeight);
    
        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(handle);
    }
}

void Application::init() {
    if (!glfwInit()) {
        throw (std::runtime_error("Failed to initialize GLFW"));
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();

    float scaleX;
    float scaleY;
    glfwGetMonitorContentScale(monitor, &scaleX, &scaleY);

    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    int screenWidth = mode->width;
    int screenHeight = mode->height;

    Rect rectPixels = rect.toPixels(
        screenWidth,
        screenHeight
    );

    if (rect.type == Rect::Type::Pixels) {
        rectPixels.width *= scaleX;
        rectPixels.height *= scaleY;
    }

    rect = rectPixels;

    handle = glfwCreateWindow(rect.width, rect.height, title.c_str(), nullptr, nullptr);
    if (!handle) {
        glfwTerminate();
        throw (std::runtime_error("Failed to create window"));
    }

    glfwMakeContextCurrent(handle);
    // glfwSwapInterval(1);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        glfwDestroyWindow(handle);
        glfwTerminate();
        throw (std::runtime_error("Failed to initialize GLAD"));
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsLight();

    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(scaleX);

    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.LogFilename = nullptr;

    io.Fonts->Clear();
    ImFont* font = io.Fonts->AddFontFromFileTTF(
        "assets/fonts/Roboto/static/Roboto-Regular.ttf",
        14.0f * scaleX
    );

    ImGui_ImplGlfw_InitForOpenGL(handle, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    ImPlot::CreateContext();
}

void Application::destroy() {
    ImPlot::DestroyContext();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    ImGui::DestroyContext();

    glfwDestroyWindow(handle);
    glfwTerminate();
}

void Application::menu() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::MenuItem("File", nullptr, tab == menuTabs::File)) {
            tab = menuTabs::File;
        }

        if (ImGui::MenuItem("Process", nullptr, tab == menuTabs::Process)) {
            tab = menuTabs::Process;
        }

        if (ImGui::MenuItem("Plots", nullptr, tab == menuTabs::Plots)) {
            tab = menuTabs::Plots;
        }

        if (ImGui::MenuItem("View", nullptr, tab == menuTabs::View)) {
            tab = menuTabs::View;
        }

        ImGui::EndMainMenuBar();
    }
}

void Application::render() {
    for (auto& widget : widgets) {
        if (widget->getTab() == static_cast<int>(tab) && widget->getVisible()) {
            widget->render();
        }
    }
}