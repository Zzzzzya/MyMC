#pragma once
#ifndef MESH_HPP
#define MESH_HPP

#include "Header.hpp"

/* 顶点类 */
struct Vertex {
    vec3 position;
    vec3 normal;
    vec2 texCoords;
};

struct Vertex2D {
    vec2 position;
    vec2 texCoords;
};

class Texture;
class Shader;
/* 网格 - 一套物体 */
class Mesh {
  public:
    /*网格数据 --> 顶点(顶点位置 + 法线 + 纹理坐标) + 索引 + 纹理的集合 */
    vector<Vertex> vertice;
    unsigned int VAO, VBO;
    mat4 model = mat4(1.0f);

    Mesh(const vector<Vertex> &vertices);
    virtual void Draw(shared_ptr<Shader> &Shader) = 0;

  private:
    void setupMesh();
};

/* --------------  基础形体类 ------------- */
/* Cube */
class Cube : public Mesh {
  public:
    Cube();
    const static std::string CubeDir;
};

/* ------------- 实际物体类 -------------- */
/* ------ Cube类物体😍 ------ */
class GrassBlock : public Cube {
  public:
    GrassBlock();
    void Draw(shared_ptr<Shader> &Shader) override;

    const static std::string GrassBlockDir;
    static vector<Texture> textures; // Cube 的纹理,六个面 ： 顺序为 ： 右 左 上 下 后 前
    static void loadTextures();
};

#endif