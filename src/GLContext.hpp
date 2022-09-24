#ifndef GLCONTEXT_HPP
#define GLCONTEXT_HPP

// clang-format off
#include <glad.h>
#include "GLFW/glfw3.h"
// clang-format on

#include <cstdio>
#include <cstdlib>

struct GLContext {
    [[nodiscard]] auto window() const noexcept -> GLFWwindow *;

    [[nodiscard]] auto ratio() const noexcept -> float;

    GLContext() noexcept;
    GLContext(const GLContext &) noexcept = delete;
    GLContext(GLContext &&) noexcept = delete;
    auto operator=(const GLContext &) noexcept -> GLContext & = delete;
    auto operator=(GLContext &&) noexcept -> GLContext & = delete;
    ~GLContext();

   private:
    GLFWwindow *m_window;
    float m_ratio;
};

#endif
