#include <vector>

#include "Entity.hpp"
#include "GLContext.hpp"
#include "Shader.hpp"

auto main() -> int {
    GLContext context;

    Ball ball{0, 0, context.ratio()};         // NOLINT
    Board player{-0.8f, 0, context.ratio()};  // NOLINT
    Board enemy{0.8f, 0, context.ratio()};    // NOLINT

    std::vector<Entity *> scene_objs{&ball, &player, &enemy};

    float d_t{}, t_point{};
    while (glfwWindowShouldClose(context.window()) == 0) {
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        auto t_curr = (float)glfwGetTime();
        d_t = t_curr - t_point;
        t_point = t_curr;

        if (glfwGetKey(context.window(), GLFW_KEY_UP) == GLFW_PRESS)
            player.y += player.speed * d_t;
        if (glfwGetKey(context.window(), GLFW_KEY_DOWN) == GLFW_PRESS)
            player.y -= player.speed * d_t;

        for (const auto &obj : scene_objs) obj->draw();

        glfwSwapBuffers(context.window());
        glfwPollEvents();
    }

    return 0;
}
