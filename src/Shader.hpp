#ifndef SHADER_HPP
#define SHADER_HPP

#include <fstream>
// #include <glm/fwd.hpp>
#include <iostream>
#include <sstream>
#include <string>

#include "glad.h"

struct Shader {
    Shader(const char *vertexPath, const char *fragmentPath);

    void use() const noexcept;

    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    // void setBool(const std::string &name, bool value) const;
    // void setMat4(const std::string &name, const glm::mat4 &mat) const;
    unsigned int ID;
};
#endif
