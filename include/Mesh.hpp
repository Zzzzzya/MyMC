#pragma once
#ifndef MESH_HPP
#define MESH_HPP

#include "Header.hpp"

/* 顶点类 */
struct Vertex {
    vec3 position;
    vec2 texCoords;

    int faceID = 0;
    int cubeID = 0;
};

struct Vertex2D {
    vec2 position;
    vec2 texCoords;
};

class Texture;
class Shader;

/* --------------  基础形体类 ------------- */
/* Cube */
struct Cube {
    int CubeID = 0;
    bool Exposed[6] = {false, false, false, false, false, false};
    static unordered_map<int, std::pair<int, int>> CubeIdMap; // CubeID -> TextureID, ShaderID
    int GetTextureID() const;
    int GetShaderID() const;
};

/* ------- Chunk -------- */
class Chunk {
  public:
    Chunk(const shared_ptr<vector<vector<vector<shared_ptr<Cube>>>>> &Map, vec3 position, vec3 size = vec3(64, 16, 64));
    unsigned int VAO, VBO;
    vec3 pos;
    vec3 size;
    shared_ptr<vector<vector<vector<shared_ptr<Cube>>>>> map;
    vector<Vertex> vertices;
    void GenerateMesh();
    void setupBuffer();
    void Draw(const mat4 &view, const mat4 &projection, float CubeMap = 2.0f);

  private:
    void init();
};

#endif