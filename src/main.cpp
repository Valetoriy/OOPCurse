// clang-format off
#include "glad.h"
#include "GLFW/glfw3.h"
// clang-format on

#include <cstdio>
#include <iostream>

#include "Shader.hpp"
#include "stb_image.hpp"

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
    else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

auto main() -> int {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    GLFWwindow *window =
        glfwCreateWindow(mode->width, mode->height, "CURSE", monitor, nullptr);
    if (window == nullptr) {
        puts("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0) {  // NOLINT
        puts("Failed to initialize GLAD");
        return -1;
    }

    glViewport(0, 0, mode->width, mode->height);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    unsigned int VAOid;
    glGenVertexArrays(1, &VAOid);
    glBindVertexArray(VAOid);

    // clang-format off
    float vertices[] = {
        -0.1f, 0.1f, 0.0f, 1.0f,  // NOLINT
        0.1f, 0.1f, 1.0f, 1.0f,  // NOLINT
        0.1f, -0.1f, 1.0f, 0.0f,  // NOLINT
        -0.1f, -0.1f, 0.0f, 0.0f  // NOLINT
    };
    // clang-format on
    unsigned short indices[] = {0, 1, 2, 0, 2, 3};
    for (int i{}; (unsigned long)i < sizeof(vertices) / sizeof(float); i += 4) {
        vertices[i] *= (float)mode->height / (float)mode->width;
    }
    unsigned int VBOid;
    glGenBuffers(1, &VBOid);
    glBindBuffer(GL_ARRAY_BUFFER, VBOid);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int EBOid;
    glGenBuffers(1, &EBOid);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOid);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          (void *)(2 * sizeof(float)));  // NOLINT

    Shader shader{"src/VS.glsl", "src/FS.glsl"};
    shader.use();

    unsigned int facetx;
    glGenTextures(1, &facetx);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, facetx);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(1);
    unsigned char *data =
        stbi_load("assets/ball.png", &width, &height, &nrChannels, 0);
    if (data != nullptr) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else
        puts("Failed to load texture image!");
    stbi_image_free(data);

    shader.setInt("tex", 0);

    while (glfwWindowShouldClose(window) == 0) {
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        processInput(window);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);  // NOLINT

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
