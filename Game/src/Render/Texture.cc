#include "mTexture.hpp"
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "stb_image.h"

static std::string path = "../res/Models/Cubes/";
static std::string QuadPath = "../res/Models/Quads/";
static vector<std::string> filename = {"/right.png", "/left.png", "/top.png", "/bottom.png", "/front.png", "/back.png"};

vector<shared_ptr<Texture>> Texture::DefaultTexture = {};

/* Default Textures */
static void setUpDefaultTexturesHelper(const std::string &CubeName) {
    Texture::DefaultTexture.push_back(make_shared<Texture>("/0.png", QuadPath + CubeName));
}

void Texture::setUpDefaultTextures() {
    // 0
    setUpDefaultTexturesHelper("Grass");
}

shared_ptr<Texture> Texture::GetDefaultTexture(int i) {
    return Texture::DefaultTexture[i];
}

/* Default CubeMaps */
vector<shared_ptr<CubeMap>> CubeMap::DefaultCubeMaps = {};
static void setUpDefaultCubeMapsHelper(const std::string &CubeName) {
    CubeMap::DefaultCubeMaps.push_back(make_shared<CubeMap>(filename, path + CubeName));
}
void CubeMap::setUpDefaultCubeMaps() {
    // 0
    setUpDefaultCubeMapsHelper("GrassBlock");
    // 1
    setUpDefaultCubeMapsHelper("DirtBlock");
    // 2
    setUpDefaultCubeMapsHelper("BedRock");
    // 3
    setUpDefaultCubeMapsHelper("Stone");
    // 4
    setUpDefaultCubeMapsHelper("Wood");
    // 5
    setUpDefaultCubeMapsHelper("Leaves");
    // 6
    setUpDefaultCubeMapsHelper("Diamond");
}
shared_ptr<CubeMap> CubeMap::GetDefaultCubeMap(int i) {
    return CubeMap::DefaultCubeMaps[i];
}

Texture::Texture() {
}

Texture::Texture(const std::string &filename, const std::string &directory) {
    loadTexture(filename, directory);
}

void Texture::loadTexture(const std::string &filename, const std::string &directory) {
    auto logger = Loggers::getLogger("Texture");
    path = directory + filename;

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    auto data = stbi_load(path.c_str(), &width, &height, &nrComponents, STBI_rgb_alpha);
    if (!data) {
        LOG_ERROR(logger, "ERROR::TEXTURE::LOAD TEXTURE FAILED ON PATH " + path);
        stbi_image_free(data);
        return;
    }
    // GLenum format = (nrComponents == 1) ? GL_RED : (nrComponents == 3 ? GL_RGB : GL_RGBA);
    GLenum format = GL_RGBA;
    unsigned int textureID;
    glGenTextures(1, &textureID);
    id = textureID;
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D); // 纹理记得生成MipMap！

    setTexParam();

    stbi_image_free(data);
    handle = glGetTextureHandleARB(textureID);
    glMakeTextureHandleResidentARB(handle);

    glBindTexture(GL_TEXTURE_2D, 0);
    return;
}

void Texture::setTexParam() {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
}

CubeMap::CubeMap(const std::vector<std::string> &faces, const std::string &directory) {
    loadCubeMap(faces, directory);
}

void CubeMap::loadCubeMap(const std::vector<std::string> &faces, const std::string &directory) {
    auto logger = Loggers::getLogger("Texture");
    stbi_set_flip_vertically_on_load(false);
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        unsigned char *data = stbi_load((directory + faces[i]).c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);
        // GLenum format = (nrChannels == 1) ? GL_RED : (nrChannels == 3 ? GL_RGB : GL_RGBA);
        GLenum format = GL_RGBA;
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE,
                         data);
            stbi_image_free(data);
        }
        else {
            LOG_ERROR(logger, std::string("Cubemap texture failed to load at path: ") + directory + faces[i]);
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    handle = glGetTextureHandleARB(textureID);
    glMakeTextureHandleResidentARB(handle);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    id = textureID;
}

Texture_HDR::Texture_HDR(const std::string &paths, const std::string &directory) {
    loadHDR(paths, directory);
}

unsigned int Texture_HDR::hdr() {
    auto logger = Loggers::getLogger("Texture");
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrComponents;
    float *data = stbi_loadf((std::string("../res/textures/HDR1.hdr")).c_str(), &width, &height, &nrComponents, 0);
    unsigned int hdrTexture;
    if (data) {
        glGenTextures(1, &hdrTexture);
        glBindTexture(GL_TEXTURE_2D, hdrTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT,
                     data); // note how we specify the texture's data value to be float

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else {
        LOG_ERROR(logger, "Failed to load HDR image.");
    }
    return hdrTexture;
}

void Texture_HDR::loadHDR(const std::string &paths, const std::string &directory) {
    auto logger = Loggers::getLogger("Texture");
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrComponents;
    auto file = directory + paths;
    float *data = stbi_loadf(file.c_str(), &width, &height, &nrComponents, 0);
    unsigned int hdrTexture;
    if (data) {
        glGenTextures(1, &hdrTexture);
        glBindTexture(GL_TEXTURE_2D, hdrTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
        id = hdrTexture;
    }
    else {
        LOG_ERROR(logger, "Failed to load HDR image.");
        id = -1;
    }

    stbi_set_flip_vertically_on_load(false);
}
