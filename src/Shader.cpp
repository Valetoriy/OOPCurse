#include "Shader.hpp"

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch (std::ifstream::failure& e) {
        puts("Ошибка при чтении файла!");
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int VSid, FSid;
    int success;
    char infoLog[256];  // NOLINT

    VSid = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VSid, 1, &vShaderCode, nullptr);
    glCompileShader(VSid);

    glGetShaderiv(VSid, GL_COMPILE_STATUS, &success);
    if (success == 0) {
        glGetShaderInfoLog(VSid, sizeof(infoLog), nullptr, infoLog);
        std::cout << "Ошибка компиляции вершинного шейдера!\n"
                  << infoLog << '\n';
    };

    FSid = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FSid, 1, &fShaderCode, nullptr);
    glCompileShader(FSid);

    glGetShaderiv(FSid, GL_COMPILE_STATUS, &success);
    if (success == 0) {
        glGetShaderInfoLog(FSid, sizeof(infoLog), nullptr, infoLog);
        std::cout << "Ошибка компиляции фрагментного шейдера!\n"
                  << infoLog << '\n';
    };

    ID = glCreateProgram();
    glAttachShader(ID, VSid);
    glAttachShader(ID, FSid);
    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (success == 0) {
        glGetProgramInfoLog(ID, sizeof(infoLog), nullptr, infoLog);
        std::cout << "Ошибка компановки программы!\n" << infoLog << '\n';
    }

    glDeleteShader(VSid);
    glDeleteShader(FSid);
}

void Shader::use() const noexcept { glUseProgram(ID); }

void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
// void Shader::setBool(const std::string& name, bool value) const {
//     glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
// }
// void Shader::setFloat(const std::string& name, float value) const {
//     glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
// }
// void Shader::setMat4(const std::string& name, const glm::mat4& mat) const {
//     glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
//                        glm::value_ptr(mat));
// }
