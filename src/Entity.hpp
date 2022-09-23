#ifndef ENTITY_HPP
#define ENTITY_HPP

// clang-format off
#include <glad.h>
#include "GLFW/glfw3.h"
// clang-format on

#include <cstdio>
#include <iostream>
#include <stb_image.hpp>

#include "Shader.hpp"

struct v2 {
    int x, y;
};

struct Entity {
    virtual auto draw() noexcept -> void = 0;

    Entity(float in_x, float in_y) noexcept : x{in_x}, y{in_y} {}
    Entity(const Entity &) noexcept = default;
    Entity(Entity &&) noexcept = default;
    auto operator=(const Entity &) noexcept -> Entity & = default;
    auto operator=(Entity &&) noexcept -> Entity & = default;
    virtual ~Entity() = default;

    float x, y;
};

struct Ball : Entity {
    auto draw() noexcept -> void override {
        glBindVertexArray(VAO);
        shader.use();
        shader.setFloat("x_o", x);
        shader.setFloat("y_o", y);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);  // NOLINT
    }

    Ball(float in_x, float in_y, float ratio)
        : Entity{in_x, in_y}, shader{"src/BallVS.glsl", "src/BallFS.glsl"} {
        unsigned int VAOid;
        glGenVertexArrays(1, &VAOid);
        VAO = VAOid;
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
        for (int i{}; (unsigned long)i < sizeof(vertices) / sizeof(float);
             i += 4) {
            vertices[i] *= ratio;
        }
        unsigned int VBOid;
        glGenBuffers(1, &VBOid);
        glBindBuffer(GL_ARRAY_BUFFER, VBOid);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                     GL_STATIC_DRAW);

        unsigned int EBOid;
        glGenBuffers(1, &EBOid);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOid);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                     GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                              nullptr);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                              (void *)(2 * sizeof(float)));  // NOLINT

        // shader.use();

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
            puts("Ошибка при загрузке изображения!");
        stbi_image_free(data);

        shader.setInt("tex", 0);
    }

    unsigned int VAO;
    Shader shader;
};

struct Board : Entity {
    auto draw() noexcept -> void override;

    Board(float in_x, float in_y) : Entity{in_x, in_y} {}
    // unsigned int VAO;
};

#endif
