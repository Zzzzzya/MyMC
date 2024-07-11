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
    PerlinNoise2D(float frequency = 10.0, float persistence = 0.02, float lacunarity = 2.0, int octaveCount = 3,
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
    shared_ptr<vector<vector<vector<shared_ptr<Cube>>>>> _map;

    int bedRockHeight = 10;
    int stoneHeight = 20;
    int dirtHeight = 5;
    PerlinNoise2D perlinNoise;
    vector<vector<int>> heightMap;

    void InitMap();
    inline shared_ptr<vector<vector<vector<shared_ptr<Cube>>>>> GetMap() const {
        return _map;
    }

  private:
    void resizeMap();
    void generateMap();
    void setExposed();

    bool CheckIfCanContainsATree(int x, int y, int z, int height);
    void GenerateATree(int x, int y, int z, int height, int type = 0);
};

#endif