#pragma once

#include <string>
#include <stdint.h>
#include <memory>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "layer.h"

namespace Jug
{
    class Application
    {
    private:
        static Application *singletonInstance;

        GLFWwindow *window;
        std::string title;
        uint32_t width;
        uint32_t height;

        float lastFrameTime = 0.0f, timeStep = 0.0f, currentFrameTime = 0.0f;

        std::vector<std::shared_ptr<Layer>> layers;

    public:
        ~Application();
        static Application *getInstance();
        static Application *createInstance(std::string title, uint32_t width, uint32_t height);

        void run();
        void setClearColor(float r, float g, float b, float a);
        void addLayer(std::shared_ptr<Layer> layer);

        GLFWwindow *getWindow();

    private:
        Application(std::string title, uint32_t width, uint32_t height);

        void init();
        bool initGLFW();
        bool initGlew();
        bool initImGui();
        void setColors();

        void shutDown();

        static void glfwErrorCallback(int error, const char *description);
        static void openglErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);
    };

} // namespace Jug
