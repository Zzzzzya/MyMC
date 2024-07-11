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
    vec3 CubeMapTex = vec3(0.0f);
};

struct Vertex2D {
    vec2 position;
    vec2 texCoords;
};

class Texture;
class Shader;
class Map;

struct Mesh {
    bool Exposed[6] = {false, false, false, false, false, false};
    virtual void GenerateVertices(vector<Vertex> &vertices, vec3 WorldPos) const = 0;
    virtual int &ID() = 0;
    virtual ~Mesh() = default;
};

/* --------------  基础形体类 ------------- */
/* Cube */
struct Cube : public Mesh {
    int CubeID = 0;
    static vector<Vertex> CubeVertice;
    static unordered_map<int, std::pair<int, int>> CubeIdMap; // CubeID -> TextureID, ShaderID
    int GetTextureID() const;
    int GetShaderID() const;
    virtual int &ID() override {
        return this->CubeID;
    }
    virtual void GenerateVertices(vector<Vertex> &vertices, vec3 WorldPos) const override;
};

/* Quad */
struct Quad : public Mesh {
    int QuadID = 0;
    static vector<Vertex2D> QuadVertice;
    static unordered_map<int, std::pair<int, int>> QuadIdMap; // QuadID -> TextureID, ShaderID
    virtual int &ID() override {
        return this->QuadID;
    }
    virtual void GenerateVertices(vector<Vertex> &vertices, vec3 WorldPos) const override;
};

/* --------------------------------- 核心数据结构  Chunk类 🫣🫣🥵🥵🥵😍😍😍😍😘😘😘😘 ------------------------------*/
/* ------- Chunk -------- */
class Chunk {
  public:
    Chunk(const shared_ptr<Map> &map, vec3 position, vec3 size = vec3(64, 16, 64));
    unsigned int VAO, VBO;
    vec3 pos;
    vec3 size;
    shared_ptr<Map> map;
    vector<Vertex> vertices;
    void GenerateMesh();
    void setupBuffer();
    void Draw(const mat4 &view, const mat4 &projection, float CubeMap = 2.0f);

  private:
    void init();
};

// 方块序号宏定义 - 参见Cube.md
#define CB_EMPTY 0
#define CB_GRASS_BLOCK 1
#define CB_DIRT_BLOCK 2
#define CB_BEDROCK 3
#define CB_STONE 4
#define CB_WOOD 5
#define CB_LEAVES 6
#define CB_DIAMOND 7
#endif