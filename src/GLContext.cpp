#include "GLContext.hpp"

#define GLT_IMPORTS
#include <gltext.h>

auto GLContext::window() const noexcept -> GLFWwindow * { return m_window; }

auto GLContext::ratio() const noexcept -> float { return m_ratio; }

GLContext::GLContext() noexcept {
    if (glfwInit() == 0) {
        puts("Ошибка при инициализации GLFW!");
        exit(1);  // NOLINT
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_FALSE);

    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);
    m_ratio = (float)mode->height / (float)mode->width;

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    m_window =
        glfwCreateWindow(mode->width, mode->height, "CURSE", monitor, nullptr);
    if (m_window == nullptr) {
        puts("Ошибка при создании окна!");
        glfwTerminate();
        exit(1);  // NOLINT
    }

    glfwMakeContextCurrent(m_window);
    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) ==  // NOLINT
        0) {
        puts("Ошибка при инициализации Glad!");
        glfwTerminate();
        exit(1);  // NOLINT
    }

    glViewport(0, 0, mode->width, mode->height);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0, 0, 0, 1);

    if (gltInit() == 0) {
        puts("Ошибка при инициализации glText!\n");
        glfwTerminate();
        exit(1);  // NOLINT
    }
}

GLContext::~GLContext() {
    gltTerminate();
    glfwTerminate();
}
