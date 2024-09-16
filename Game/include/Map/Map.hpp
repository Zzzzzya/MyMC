#pragma once

#ifndef MAP_HPP
#define MAP_HPP

#include "Header.hpp"
#include "Mesh.hpp"
#include "noise/noise.h"

class PerlinNoise2D {
  private:
    noise::module::Perlin perlinModule;

  public:
    PerlinNoise2D(float frequency = 10.0, float persistence = 0.5, float lacunarity = 2.0, int octaveCount = 3,
                  noise::NoiseQuality noiseQuality = noise::QUALITY_STD) {
        // 初始化 Perlin 噪声模块的参数
        perlinModule.SetFrequency(frequency);       // 设置频率
        perlinModule.SetPersistence(persistence);   // 设置持续度
        perlinModule.SetLacunarity(lacunarity);     // 设置细节层次的频率变化
        perlinModule.SetOctaveCount(octaveCount);   // 设置八度数
        perlinModule.SetNoiseQuality(noiseQuality); // 设置噪声质量
    }

    // 生成并返回给定坐标的噪声值
    double GetValue(double x, double y) const {
        double value = perlinModule.GetValue(x, 0, y);
        return std::fmax(0.0f, std::fmin(1.0f, value));
    }

    // 生成多层次的噪声以创建复杂地形
    double GenerateTerrain(double x, double y) const {
        double total = 0.0;
        double amplitude = 1.0;
        double frequency = 1.0;

        for (int i = 0; i < perlinModule.GetOctaveCount(); ++i) {
            total += perlinModule.GetValue(x * frequency, 0, y * frequency) * amplitude;
            amplitude *= perlinModule.GetPersistence();
            frequency *= perlinModule.GetLacunarity();
        }

        double value = total * (1.0 - perlinModule.GetPersistence()) / (1.0 - amplitude);
        return std::fmax(0.0f, std::fmin(1.0f, value));
    }
};

class Map {
  public:
    Map(vec3 mapSize = vec3(20, 20, 20), int seed = 0);

    int seed;
    vec3 mapSize;
    shared_ptr<vector<vector<vector<shared_ptr<Mesh>>>>> _map;

    int bedRockHeight = 10;
    int stoneHeight = 20;
    int dirtHeight = 5;
    PerlinNoise2D perlinNoise;
    PerlinNoise2D noiseLayer1;
    PerlinNoise2D noiseLayer2;
    PerlinNoise2D noiseLayer3;
    vector<vector<int>> heightMap;

    CloudChunk cloudChunk;

    void InitMap();
    inline shared_ptr<vector<vector<vector<shared_ptr<Mesh>>>>> GetMap() const {
        return _map;
    }
    static inline vec3 GetBlockCoords(const vec3 &pos) {
        auto thePos = pos + vec3(1.0f, 1.0f, -1.0f);
        thePos.z = -thePos.z;
        thePos /= 2.0f;
        return vec3(floor(thePos.x), floor(thePos.y), floor(thePos.z));
    }
    bool CheckCollision(const vec3 &position, vec3 &movVec) const;
    bool CheckCollisionSingle(const vec3 &position) const;
    bool CheckCollisionHelper(const vec3 &pos) const;
    bool CheckHaveSomething(const vec3 &pos) const;
    bool ViewRayTrace(const vec3 &position, const vec3 &direction, vec3 &ToDo, vec3 &ToAdd, float dis = 10.0f,
                      float step = 0.1f) const;

  private:
    void resizeMap();
    void generateMap();
    void setExposed();

    // 地表物体生成
    void GenerateEarth();
    void GenerateSurface();
    void GenerateTrees();
    void GenerateGrass();
    void GenerateClouds();

    bool CheckIfCanContainsATree(int x, int y, int z, int height);
    void GenerateATree(int x, int y, int z, int height, int type = 0);
};

#endif