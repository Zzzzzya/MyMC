#pragma once
#ifndef MESH_HPP
#define MESH_HPP

#include "Header.hpp"

// 方块序号宏定义 - 参见Cube.md
#define CB_EMPTY 0
#define CB_GRASS_BLOCK 1
#define CB_DIRT_BLOCK 2
#define CB_BEDROCK 3
#define CB_STONE 4
#define CB_WOOD 5
#define CB_LEAVES 6
#define CB_DIAMOND 7
#define CB_SAND 8

#define CB_CLOUD 17
#define CB_WATER 18

// 四边形纹理宏定义
#define QD_EMPTY 0
#define QD_GRASS 1

/* 顶点类 */
struct Vertex {
    vec3 position;
    vec2 texCoords;
    vec3 normal = vec3(0.0f, 1.0f, 0.0f);

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
    virtual bool Occluded() const = 0;
    virtual bool Passed() const = 0;
    virtual bool RayPassed() const = 0;
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
    virtual bool Occluded() const override {
        return CubeID && (CubeID - CB_WATER);
    }
    virtual int &ID() override {
        return this->CubeID;
    }
    virtual bool Passed() const override {
        return !CubeID || CubeID == CB_WATER;
    }
    virtual bool RayPassed() const override {
        return Passed();
    }
    virtual void GenerateVertices(vector<Vertex> &vertices, vec3 WorldPos) const override;
};

/* Quad */
struct Quad : public Mesh {
    int QuadID = 0;
    static vector<Vertex> QuadVertice;
    static unordered_map<int, std::pair<int, int>> QuadIdMap; // QuadID -> TextureID, ShaderID
    virtual bool Occluded() const override {
        return false;
    }
    virtual int &ID() override {
        return this->QuadID;
    }
    virtual bool Passed() const override {
        return !QuadID;
    }
    virtual bool RayPassed() const override {
        return Passed();
    }
    virtual void GenerateVertices(vector<Vertex> &vertices, vec3 WorldPos) const override;
};

/* CrossQuad */
struct CrossQuad : public Mesh {
    int CrossQuadID = 0;
    static vector<Vertex> CrossQuadVertice;
    static unordered_map<int, std::pair<int, int>> CrossQuadIdMap; // CrossQuadID -> TextureID, ShaderID
    virtual bool Occluded() const override {
        return false;
    }
    virtual int &ID() override {
        return this->CrossQuadID;
    }
    virtual bool Passed() const override {
        return true;
    }
    virtual bool RayPassed() const override {
        return false;
    }

    virtual void GenerateVertices(vector<Vertex> &vertices, vec3 WorldPos) const override;
};

/* --------------------------------- 核心数据结构  Chunk类 🫣🫣🥵🥵🥵😍😍😍😍😘😘😘😘 ------------------------------*/
/* ------- Chunk -------- */
class Chunk {
  public:
    Chunk(const shared_ptr<Map> &map, vec3 position, vec3 size = vec3(64, 16, 64));
    unsigned int VAO, VBO;
    unsigned int CloudVAO, CloudVBO;
    vec3 pos;
    vec3 size;
    int verSize = 0;
    int CloudVersize = 0;
    shared_ptr<Map> map;
    vector<Vertex> vertices;
    vector<Vertex> CloudVertices;
    bool isCulled = false; // 是否被剔除
    void GenerateMesh();
    void setupBuffer();
    void Draw(const mat4 &view, const mat4 &projection, float CubeMap = 2.0f);

  private:
    void init();
};

class CloudChunk {
  public:
    unsigned int CloudVAO, CloudVBO;
    vector<Vertex> CloudVertices;
    int CloudVersize = 0;
    void setupBuffer();
    void Draw(const mat4 &view, const mat4 &projection, float CubeMap = 2.0f);
};

class SunChunk {
  public:
    unsigned int SunVAO, SunVBO;
    vector<Vertex> SunVertices;
    int SunVersize = 0;
    void init();
    void setupBuffer();
    void Draw(const mat4 &view, const mat4 &projection, float CubeMap = 2.0f);
};

class WaterChunk {
  public:
    unsigned int WaterVAO, WaterVBO;
    vector<Vertex> WaterVertices;
    int WaterVersize = 0;
    void init();
    void setupBuffer();
    void Draw(const mat4 &view, const mat4 &projection, float CubeMap = 2.0f);
};

class ScreenQuad {
  public:
    ScreenQuad();
    unsigned int VAO, VBO;
    vector<Vertex> vertices;
    void init();
    void Draw();

  private:
    void GenerateMesh();
    void setupBuffer();
};

class ScreenMesh {
  public:
    ScreenMesh();
    unsigned int VAO, VBO;
    vector<Vertex2D> vertices;
    void init();
    void Draw();

  private:
    void GenerateMesh();
    void setupBuffer();
};

#endif