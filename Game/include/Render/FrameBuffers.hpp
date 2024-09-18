#pragma once
#ifndef FRAMEBUFFERS_HPP
#define FRAMEBUFFERS_HPP

#include "Header.hpp"

class FrameBuffer {
  public:
    FrameBuffer(int, int);
    unsigned int FBO;
    unsigned int tex;
    unsigned int rbo;
    unsigned int depthTex;
    unsigned int normalTex;
    GLuint depthStencilTex;

    void setUpFrameBuffer(int, int);
};

class FrameBufferDepthMap {
  public:
    FrameBufferDepthMap(int, int);
    unsigned int FBO;
    unsigned int tex;

    void setUpFrameBuffer(int, int);
};

class FrameBufferOnlyRBO {
  public:
    FrameBufferOnlyRBO(int, int);
    unsigned int FBO;
    unsigned int tex;
    unsigned int rbo;

    void setUpFrameBuffer(int, int);
};

#endif