#include <numbers>
#include <random>
#include <vector>

#define GLT_IMPLEMENTATION
#include "Entity.hpp"
#include "GLContext.hpp"
#include "Shader.hpp"

auto main() -> int {  // NOLINT
    GLContext context;

    Ball ball{0, 0, context.ratio(), 0.05f};               // NOLINT
    Board player{-0.8f, 0, context.ratio(), 0.02f, 0.2f};  // NOLINT
    Board enemy{0.8f, 0, context.ratio(), 0.02f, 0.2f};    // NOLINT
    Text wins{0, 0, 10, 0, 0, 1};                          // NOLINT
    Text losses{0, 120, 10, 0, 1, 0};                      // NOLINT

    std::vector<Entity *> scene_objs{&ball, &player, &enemy, &wins, &losses};

    std::random_device r_d;
    std::mt19937 gen{r_d()};
    std::uniform_real_distribution dist{-30.0f, 30.0f};  // NOLINT
    auto f_pi = std::numbers::pi_v<float>;
    auto ResetGame = [&]() {
        player.y = 0;

        ball.x = 0;
        ball.y = 0;
        float rand_deg{dist(gen)};
        ball.dx =
            cosf(rand_deg / 180 * f_pi) * 0.015f * context.ratio();  // NOLINT
        ball.dy = sinf(rand_deg / 180 * f_pi) * 0.015f;              // NOLINT

        enemy.y = 0;
    };

    ResetGame();

    float d_t{}, t_point{}, timer{};
    while (glfwWindowShouldClose(context.window()) == 0) {
        glClear(GL_COLOR_BUFFER_BIT);

        auto t_curr = (float)glfwGetTime();
        d_t = t_curr - t_point;
        timer += d_t;
        t_point = t_curr;

        // Обновляем позицию игрока
        if (glfwGetKey(context.window(), GLFW_KEY_UP) == GLFW_PRESS and
            player.y < 1 - player.height())
            player.y += player.speed() * d_t;
        if (glfwGetKey(context.window(), GLFW_KEY_DOWN) == GLFW_PRESS and
            player.y > player.height() - 1)
            player.y -= player.speed() * d_t;
        if (glfwGetKey(context.window(), GLFW_KEY_SPACE) == GLFW_PRESS)
            ResetGame();

        // Обновляем позицию шара
        ball.x += ball.dx;
        ball.y += ball.dy;

        // Обновляем позицию противника
        if (ball.y > enemy.height() - 1 and ball.y < 1 - enemy.height()) {
            float diff = ball.y - enemy.y > 0 ? 1 : -1;
            enemy.y += enemy.speed() * 0.7f * diff * d_t;  // NOLINT
        }

        // Отскок мяча от вехрней и нижней частей экрана
        if (ball.y > 1 - ball.radius() or ball.y < ball.radius() - 1)
            ball.dy *= -1;

        // Проверка на столкновение мяча с игроками
        for (const auto &rect : {player, enemy}) {
            auto distx{std::abs(ball.x - rect.x)};
            auto disty{std::abs(ball.y - rect.y)};

            if (timer > 0.3) {  // NOLINT
                bool hit = false;

                // Л и П
                if (distx <= rect.width() + ball.radius() * context.ratio() and
                    disty <= rect.height()) {
                    ball.dx *= -1;
                    hit = true;
                    // В и Н
                } else if (disty <= rect.height() + ball.radius() and
                           distx <= rect.width()) {
                    ball.dy *= -1;
                    hit = true;
                }

                if (hit) {
                    timer = 0;

                    // При отскоке от игрока мяч случайно отклоняется
                    float rand_deg = dist(gen);
                    ball.dx = ball.dx * cosf(rand_deg / 180 * f_pi) -  // NOLINT
                              ball.dy * sinf(rand_deg / 180 * f_pi);   // NOLINT
                    ball.dy = ball.dy * cosf(rand_deg / 180 * f_pi) +  // NOLINT
                              ball.dx * sinf(rand_deg / 180 * f_pi);   // NOLINT

                    // И ускоряется
                    ball.dx *= 1.1f;  // NOLINT
                    ball.dy *= 1.1f;  // NOLINT
                }
            }
        }

        // Проверка на пропуск мяча
        if (ball.x < player.x - player.width()) {
            ++losses;
            ResetGame();
        }
        if (ball.x > enemy.x + enemy.width()) {
            ++wins;
            ResetGame();
        }

        // Игра идёт до 10 очков
        if (wins.value() > 9) {  // NOLINT
            puts("Вы победили!");
            return 0;
        }
        if (losses.value() > 9) {  // NOLINT
            puts("Вы проиграли!");
            return 0;
        }

        for (const auto &obj : scene_objs) obj->draw();

        glfwSwapBuffers(context.window());
        glfwPollEvents();
    }

    return 0;
}
