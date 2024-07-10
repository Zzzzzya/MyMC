#pragma once

#ifndef SHADER_HPP
#define SHADER_HPP

#include "Header.hpp"

// Shader推荐使用shared_ptr
class Shader {
  public:
    unsigned int pro = -1;
    Shader() = default;
    Shader(const std::string &vertexPath, const std::string &fragmentPath, bool hasGeo = false,
           const std::string &geoName = "null", const std::string &vertexDirectory = "../shaders/vertex/",
           const std::string &fragmentDirectory = "../shaders/fragment/",
           const std::string &geoDirectory = "../shaders/geo/");
    void use();

    // Uniform functions
    // 1
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;

    // 2
    void setVec2(const std::string &name, const glm::vec2 &value) const;

    // 3
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setMat3(const std::string &name, const glm::mat3 &value) const;

    // 4
    void setVec4(const std::string &name, const glm::vec4 &value) const;
    void setMat4(const std::string &name, const glm::mat4 &value) const;

    // Uniforms
    void setMVPS(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection);

    // Shaders
    static vector<shared_ptr<Shader>> DefaultShader;
    static void setUpDefaultShaders();
    static shared_ptr<Shader> GetDefaultShader(int i);

  private:
    void SetUpShader(const std::string &vertexName, const std::string &fragmentName, const std::string &vertexDirectory,
                     const std::string &fragmentDirectory);
    void SetUpShader2(const std::string &vertexName, const std::string &fragmentName, const std::string &geoName,
                      const std::string &vertexDirectory, const std::string &fragmentDirectory,
                      const std::string &geoDirectory);
};

#endif