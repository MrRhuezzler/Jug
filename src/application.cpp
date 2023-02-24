#include <iostream>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "glm/glm.hpp"

#include "jug/application.h"

namespace Jug
{
    Application *Application::getInstance()
    {
        return singletonInstance;
    }

    Application *Application::createInstance(std::string title, uint32_t width, uint32_t height)
    {
        singletonInstance = new Application(title, width, height);
        return singletonInstance;
    }

    void Application::run()
    {

        setClearColor(0, 0, 0, 1);
        ImGuiIO &io = ImGui::GetIO();
        ImGuiStyle &style = ImGui::GetStyle();

        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();

            for (auto &layer : layers)
            {
                layer->OnUpdate(timeStep);
            }

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            {
                // float minWinSizeX = style.WindowMinSize.x;
                // float minWinSizeY = style.WindowMinSize.y;
                // style.WindowMinSize.x = 370.0f;
                // style.WindowMinSize.y = 500.0f;
                const ImGuiViewport *viewport = ImGui::GetMainViewport();
                ImGui::SetNextWindowPos(viewport->WorkPos);
                ImGui::SetNextWindowSize(viewport->WorkSize);
                ImGui::SetNextWindowViewport(viewport->ID);

                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
                ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoBringToFrontOnFocus | // we just want to use this window as a host for the menubar and docking
                                               ImGuiWindowFlags_NoNavFocus |            // so turn off everything that would make it act like a window
                                               ImGuiWindowFlags_NoDocking |
                                               ImGuiWindowFlags_NoTitleBar |
                                               ImGuiWindowFlags_NoResize |
                                               ImGuiWindowFlags_NoMove |
                                               ImGuiWindowFlags_NoCollapse |
                                               ImGuiWindowFlags_NoBackground;

                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
                ImGui::Begin("Main", nullptr, windowFlags);
                ImGui::PopStyleVar();
                ImGui::PopStyleVar(2);

                ImGui::DockSpace(ImGui::GetID("Dockspace"), ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

                // style.WindowMinSize.x = minWinSizeX;
                // style.WindowMinSize.y = minWinSizeY;

                for (auto &layer : layers)
                {
                    layer->OnUIRender();
                }

                ImGui::End();
            }

            ImGui::Render();

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                GLFWwindow *backup_current_context = glfwGetCurrentContext();
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                glfwMakeContextCurrent(backup_current_context);
            }

            float time = (float)glfwGetTime();
            currentFrameTime = time - lastFrameTime;
            timeStep = glm::min<float>(currentFrameTime, 0.03f);
            lastFrameTime = currentFrameTime;

            glfwSwapBuffers(window);
        }
    }

    void Application::setClearColor(float r, float g, float b, float a)
    {
        glClearColor(r, g, b, a);
    }

    void Application::addLayer(std::shared_ptr<Layer> layer)
    {
        layers.emplace_back(layer);
        layer->OnAttach();
    }

    GLFWwindow *Application::getWindow()
    {
        return window;
    }

    Application::Application(std::string title, uint32_t width, uint32_t height)
        : title(title), width(width), height(height)
    {
        init();
    }

    Application::~Application()
    {
        shutDown();
    }

    void Application::init()
    {
        std::cout << "Initializing GLFW" << std::endl;
        if (!initGLFW())
            exit(1);

        std::cout << "Initializing GLEW" << std::endl;
        if (!initGlew())
            exit(1);

        std::cout << "Initializing ImGUI" << std::endl;
        if (!initImGui())
            exit(1);
    }

    bool Application::initGLFW()
    {
        if (!glfwInit())
        {
            std::cerr << "[Core] Couldn't initialize GLFW" << std::endl;
            return false;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
        // glfwWindowHint(GLFW_DECORATED, false);

        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode = glfwGetVideoMode(monitor);

        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

        // window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        window = glfwCreateWindow(mode->width, mode->height, title.c_str(), monitor, NULL);
        if (!window)
        {
            std::cerr << "[GLFW] Couldn't create a GLFW window" << std::endl;
            return false;
        }

        glfwMakeContextCurrent(window);

        glfwSwapInterval(1);
        glfwSetErrorCallback(Application::glfwErrorCallback);
        return true;
    }

    bool Application::initGlew()
    {
        GLenum err = glewInit();
        if (err != GLEW_OK)
        {
            std::cerr << "[GLEW] " << glewGetErrorString(err) << std::endl;
            return false;
        }

        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(Application::openglErrorCallback, window);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
        return true;
    }

    bool Application::initImGui()
    {
        const char *glslVersion = "#version 130";
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void)io;

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        io.FontDefault = io.Fonts->AddFontFromFileTTF("./assests/poppins/Poppins-Regular.ttf", 18.0f);

        // ImGui::StyleColorsDark();
        setColors();

        ImGuiStyle &style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glslVersion);

        return true;
    }

    void Application::setColors()
    {
        // auto &colors = ImGui::GetStyle().Colors;
        ImGui::StyleColorsDark();
    }

    void Application::shutDown()
    {
        for (auto &layer : layers)
        {
            layer->OnDetach();
        }

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void Application::glfwErrorCallback(int error, const char *description)
    {
        std::cerr << "[GLFW (error : " << error << " )] " << description << std::endl;
    }

    void GLAPIENTRY Application::openglErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
    {
        std::cerr << "[OpenGL (error : " << type << ")] " << message << std::endl;
    }

    Application *Application::singletonInstance = nullptr;
}