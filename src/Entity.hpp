#ifndef ENTITY_HPP
#define ENTITY_HPP

// clang-format off
#include <glad.h>
#include "GLFW/glfw3.h"
// clang-format on

#define GLT_IMPORTS
#include <gltext.h>

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

    [[nodiscard]] auto radius() const noexcept -> float;

    Ball(float in_x, float in_y, float ratio, float in_r) noexcept;

   private:
    unsigned int m_VAO;
    unsigned int m_TEX;
    Shader m_shader;
    float m_radius;

   public:
    float dx{}, dy{};
};

struct Board : Entity {
    auto draw() const noexcept -> void override;

    [[nodiscard]] auto width() const noexcept -> float;

    [[nodiscard]] auto height() const noexcept -> float;

    [[nodiscard]] auto speed() const noexcept -> float;

    Board(float in_x, float in_y, float ratio, float h_w, float h_h) noexcept;

   private:
    unsigned int m_VAO;
    Shader m_shader;
    float m_speed;
    float m_width, m_height;
};

struct Text : Entity {
    auto draw() const noexcept -> void override;

    [[nodiscard]] auto value() const noexcept -> unsigned int;

    auto operator++() noexcept -> void;

    // В отличие от остальных структур, x и y отсчитываются с левого верхнего
    // угла экрана
    Text(float in_x, float in_y, float in_s, unsigned int value, float red,
         float green) noexcept;

    Text(const Text &) noexcept = delete;
    Text(Text &&) noexcept = delete;
    auto operator=(const Text &) noexcept -> Text & = delete;
    auto operator=(Text &&) noexcept -> Text & = delete;

    ~Text() override;

   private:
    GLTtext *m_txt;
    float m_scale;
    unsigned int m_val;
    float m_color[2];
};

#endif
