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

struct Entity {
    virtual auto draw() const noexcept -> void = 0;

    Entity(float in_x, float in_y) noexcept;
    Entity(const Entity &) noexcept = default;
    Entity(Entity &&) noexcept = default;
    auto operator=(const Entity &) noexcept -> Entity & = default;
    auto operator=(Entity &&) noexcept -> Entity & = default;
    virtual ~Entity() = default;

    float x, y;
};

struct Ball : Entity {
    auto draw() const noexcept -> void override;

    Ball(float in_x, float in_y, float ratio);

    unsigned int VAO;
    Shader shader;
    float dx{}, dy{};
};

struct Board : Entity {
    auto draw() const noexcept -> void override;

    Board(float in_x, float in_y, float ratio);
    unsigned int VAO;
    Shader shader;
    float speed;
};

#endif
