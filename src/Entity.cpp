#include "Entity.hpp"

Entity::Entity(float in_x, float in_y) noexcept : x{in_x}, y{in_y} {}

auto Ball::draw() const noexcept -> void {
    glBindVertexArray(m_VAO);
    glBindTexture(GL_TEXTURE_2D, m_TEX);
    m_shader.use();
    m_shader.setFloat("x_o", x);
    m_shader.setFloat("y_o", y);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);  // NOLINT
}

auto Ball::radius() const noexcept -> float { return m_radius; }

Ball::Ball(float in_x, float in_y, float ratio, float in_r) noexcept
    : Entity{in_x, in_y},
      m_shader{"src/BallVS.glsl", "src/BallFS.glsl"},
      m_radius{in_r} {
    unsigned int VAOid;
    glGenVertexArrays(1, &VAOid);
    m_VAO = VAOid;
    glBindVertexArray(VAOid);

    // clang-format off
        float vertices[] = {
            -m_radius * ratio, m_radius, 0.0f, 1.0f,  // NOLINT
            m_radius * ratio, m_radius, 1.0f, 1.0f,   // NOLINT
            m_radius * ratio, -m_radius, 1.0f, 0.0f,  // NOLINT
            -m_radius * ratio, -m_radius, 0.0f, 0.0f  // NOLINT
        };
    // clang-format on
    unsigned short indices[] = {0, 1, 2, 0, 2, 3};
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

    unsigned int balltx;
    glGenTextures(1, &balltx);
    m_TEX = balltx;
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, balltx);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
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

    m_shader.setInt("tex", 1);
}

auto Board::draw() const noexcept -> void {
    glBindVertexArray(m_VAO);
    m_shader.use();
    m_shader.setFloat("x_o", x);
    m_shader.setFloat("y_o", y);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);  // NOLINT
}

auto Board::width() const noexcept -> float { return m_width; }

auto Board::height() const noexcept -> float { return m_height; }

auto Board::speed() const noexcept -> float { return m_speed; }

Board::Board(float in_x, float in_y, float ratio, float h_w, float h_h) noexcept
    : Entity{in_x, in_y},
      m_shader{"src/BoardVS.glsl", "src/BoardFS.glsl"},
      m_speed{1.8f},  // NOLINT
      m_width{h_w * ratio},
      m_height{h_h} {
    unsigned int VAOid;
    glGenVertexArrays(1, &VAOid);
    m_VAO = VAOid;
    glBindVertexArray(VAOid);

    float vertices[] = {-m_width, h_h,  m_width,  h_h,
                        m_width,  -h_h, -m_width, -h_h};
    unsigned short indices[] = {0, 1, 2, 0, 2, 3};
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
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
}

auto Text::draw() const noexcept -> void {
    gltBeginDraw();

    char str[30];               // NOLINT
    sprintf(str, "%u", m_val);  // NOLINT
    gltSetText(m_txt, str);

    gltColor(m_color[0], m_color[1], 0, 1);
    gltDrawText2D(m_txt, x, y, m_scale);

    gltEndDraw();
}

auto Text::value() const noexcept -> unsigned int { return m_val; }

auto Text::operator++() noexcept -> void { ++m_val; }

Text::Text(float in_x, float in_y, float in_s, unsigned int value, float red,
           float green) noexcept
    : Entity{in_x, in_y},
      m_txt{gltCreateText()},
      m_scale{in_s},
      m_val{value},
      m_color{red, green} {}

Text::~Text() { gltDeleteText(m_txt); }
