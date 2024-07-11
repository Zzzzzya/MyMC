#pragma once
#ifndef TEXTURE
#define TEXTURE

#include "Header.hpp"

class Texture {
  public:
    unsigned int id = -1;
    std::string type = "";
    std::string path;
    int wrapS = GL_REPEAT;
    int wrapT = GL_REPEAT;
    int minFilter = GL_LINEAR_MIPMAP_LINEAR;
    int magFilter = GL_LINEAR;

    Texture();
    Texture(const std::string &filename, const std::string &directory = std::string("../res/textures/"));
    void setTexParam();

    static vector<shared_ptr<Texture>> DefaultTexture;
    static void setUpDefaultTextures();
    static shared_ptr<Texture> GetDefaultTexture(int i);

  private:
    void loadTexture(const std::string &filename, const std::string &directory);
};

class CubeMap {
  public:
    unsigned int id = -1;
    GLuint64 handle = 0;

    CubeMap(const std::vector<std::string> &paths, const std::string &directory = std::string("../res/textures/"));

    static vector<shared_ptr<CubeMap>> DefaultCubeMaps;
    static void setUpDefaultCubeMaps();
    static shared_ptr<CubeMap> GetDefaultCubeMap(int i);

  private:
    void loadCubeMap(const std::vector<std::string> &paths, const std::string &directory);
};

class Texture_HDR {
  public:
    unsigned int id = -1;

    Texture_HDR(const std::string &paths, const std::string &directory = std::string("../res/textures/"));
    static unsigned int hdr();

  private:
    void loadHDR(const std::string &paths, const std::string &directory);
};

#endif