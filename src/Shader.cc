#include "Shader.hpp"

#include <GL/glew.h>

#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const std::string &vertexName, const std::string &fragmentName, bool hasGeo, const std::string &geoName,
               const std::string &vertexDirectory, const std::string &fragmentDirectory,
               const std::string &geoDirectory) {
    if (!hasGeo)
        SetUpShader(vertexName, fragmentName, vertexDirectory, fragmentDirectory);
    else
        SetUpShader2(vertexName, fragmentName, geoName, vertexDirectory, fragmentDirectory, geoDirectory);
}

void Shader::use() {
    glUseProgram(pro);
}

void Shader::setBool(const std::string &name, bool value) const {
    auto loc = glGetUniformLocation(pro, name.c_str());
    glUniform1i(loc, value);
}

void Shader::setInt(const std::string &name, int value) const {
    auto loc = glGetUniformLocation(pro, name.c_str());
    glUniform1i(loc, value);
}

void Shader::setFloat(const std::string &name, float value) const {
    auto loc = glGetUniformLocation(pro, name.c_str());
    glUniform1f(loc, value);
}

void Shader::setVec2(const std::string &name, const glm::vec2 &value) const {
    auto loc = glGetUniformLocation(pro, name.c_str());
    glUniform2fv(loc, 1, &value[0]);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const {
    auto loc = glGetUniformLocation(pro, name.c_str());
    glUniform3fv(loc, 1, &value[0]);
}

void Shader::setMat3(const std::string &name, const glm::mat3 &value) const {
    auto loc = glGetUniformLocation(pro, name.c_str());
    glUniformMatrix3fv(loc, 1, GL_FALSE, &value[0][0]);
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value) const {
    auto loc = glGetUniformLocation(pro, name.c_str());
    glUniform4fv(loc, 1, &value[0]);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &value) const {
    auto loc = glGetUniformLocation(pro, name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, &value[0][0]);
}

void Shader::setMVPS(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection) {
    this->setMat4("model", model);
    this->setMat4("view", view);
    this->setMat4("projection", projection);
    this->setMat3("normalMatrix", glm::transpose(glm::inverse(model)));
}

// 如果没有几何着色器 只有顶点 + 片段
void Shader::SetUpShader(const std::string &vertexName, const std::string &fragmentName,
                         const std::string &vertexDirectory, const std::string &fragmentDirectory) {
    auto vertexPath = vertexDirectory + vertexName;
    auto fragmentPath = fragmentDirectory + fragmentName;

    std::ifstream vertexFile;
    std::ifstream fragmentFile;
    std::string vertexCode;
    std::string fragmentCode;

    vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vertexFile.open(vertexPath);
        fragmentFile.open(fragmentPath);

        std::stringstream vertexStream;
        vertexStream << vertexFile.rdbuf();
        vertexFile.close();
        vertexCode = vertexStream.str();

        std::stringstream fragmentStream;
        fragmentStream << fragmentFile.rdbuf();
        fragmentFile.close();
        fragmentCode = fragmentStream.str();
    }
    catch (std::ifstream::failure &e) {
        std::clog << "ERROR::SHADER::FILE NOT SUCCESSFULLY READ:" << e.what() << std::endl;
    }

    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    const char *vertexShaderSource = vertexCode.c_str();
    glShaderSource(vertex, 1, &vertexShaderSource, NULL);

    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fragmentShaderSource = fragmentCode.c_str();
    glShaderSource(fragment, 1, &fragmentShaderSource, NULL);

    int success;
    char infoLog[512] = {0};
    // Compile
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::clog << "ERROR::COMPILE VERTEX SHADER" << std::endl;
        std::clog << infoLog << std::endl;
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        return;
    }

    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::clog << "ERROR::COMPILE FRAGMENT SHADER" << std::endl;
        std::clog << infoLog << std::endl;
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        return;
    }

    // Link
    pro = glCreateProgram();
    glAttachShader(pro, vertex);
    glAttachShader(pro, fragment);
    glLinkProgram(pro);
    glGetProgramiv(pro, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(pro, 512, NULL, infoLog);
        std::clog << "Shader: " << pro << std::endl;
        std::clog << "ERROR::LINKING PROGRAM" << std::endl;
        std::clog << infoLog << std::endl;
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        glDeleteProgram(pro);
        pro = -1;
        return;
    }
}

// 如果有几何着色器
void Shader::SetUpShader2(const std::string &vertexName, const std::string &fragmentName, const std::string &geoName,
                          const std::string &vertexDirectory, const std::string &fragmentDirectory,
                          const std::string &geoDirectory) {
    auto vertexPath = vertexDirectory + vertexName;
    auto fragmentPath = fragmentDirectory + fragmentName;
    auto geoPath = geoDirectory + geoName;

    std::ifstream vertexFile;
    std::ifstream fragmentFile;
    std::ifstream geoFile;
    std::string vertexCode;
    std::string fragmentCode;
    std::string geoCode;

    vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    geoFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vertexFile.open(vertexPath);
        fragmentFile.open(fragmentPath);
        geoFile.open(geoPath);

        std::stringstream vertexStream;
        vertexStream << vertexFile.rdbuf();
        vertexFile.close();
        vertexCode = vertexStream.str();

        std::stringstream fragmentStream;
        fragmentStream << fragmentFile.rdbuf();
        fragmentFile.close();
        fragmentCode = fragmentStream.str();

        std::stringstream geoStream;
        geoStream << geoFile.rdbuf();
        geoFile.close();
        geoCode = geoStream.str();
    }
    catch (std::ifstream::failure &e) {
        std::clog << "ERROR::SHADER::FILE NOT SUCCESSFULLY READ:" << e.what() << std::endl;
    }

    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    const char *vertexShaderSource = vertexCode.c_str();
    glShaderSource(vertex, 1, &vertexShaderSource, NULL);

    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fragmentShaderSource = fragmentCode.c_str();
    glShaderSource(fragment, 1, &fragmentShaderSource, NULL);

    unsigned int geo = glCreateShader(GL_GEOMETRY_SHADER);
    const char *geoShaderSource = geoCode.c_str();
    glShaderSource(geo, 1, &geoShaderSource, NULL);

    int success;
    char infoLog[512] = {0};
    // Compile
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::clog << "ERROR::COMPILE VERTEX SHADER" << std::endl;
        std::clog << infoLog << std::endl;
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        return;
    }

    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::clog << "ERROR::COMPILE FRAGMENT SHADER" << std::endl;
        std::clog << infoLog << std::endl;
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        return;
    }

    glCompileShader(geo);
    glGetShaderiv(geo, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(geo, 512, NULL, infoLog);
        std::clog << "ERROR::COMPILE geo SHADER" << std::endl;
        std::clog << infoLog << std::endl;
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        return;
    }

    // Link
    pro = glCreateProgram();
    glAttachShader(pro, vertex);
    glAttachShader(pro, geo);
    glAttachShader(pro, fragment);
    glLinkProgram(pro);
    glGetProgramiv(pro, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(pro, 512, NULL, infoLog);
        std::clog << "Shader: " << pro << std::endl;
        std::clog << "ERROR::LINKING PROGRAM" << std::endl;
        std::clog << infoLog << std::endl;
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        glDeleteProgram(pro);
        pro = -1;
        return;
    }
}
