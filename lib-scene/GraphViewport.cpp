#include "GraphViewport.h"
#include <cstdio>


GraphViewport::GraphViewport(GraphView initialView)
    : scene(initialView) {}

GraphViewport::~GraphViewport() {
    deleteFramebuffer();
    if (shader) {
        shader->terminate();
        delete shader;
        shader = nullptr;
    }
}

bool GraphViewport::init() {
    if (initialized) return true;

    try {
        shader = new Shader("shader.vs", "shader.fs");
        initialized = true;
        // Move prints to loglines
        printf("[GraphViewport] Shaders loaded successfully\n");
        return true;
    } catch (...) {
        printf("[GraphViewport] Failed to load shaders\n");
        return false;
    }
}

// FBO management
void GraphViewport::createFramebuffer(int w, int h) {
    // Color texture attachment
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenTextures(1, &colorTexture);
    glBindTexture(GL_TEXTURE_2D, colorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, colorTexture, 0);

    // Depth / stencil renderbuffer
    glGenRenderbuffers(1, &depthRbo);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                              GL_RENDERBUFFER, depthRbo);

    // Verify completeness
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        // Move to loglines
        printf("[GraphViewport] WARNING: Framebuffer incomplete!\n");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    fbWidth  = w;
    fbHeight = h;
}

void GraphViewport::deleteFramebuffer() {
    if (colorTexture) { glDeleteTextures(1, &colorTexture);      colorTexture = 0; }
    if (depthRbo)     { glDeleteRenderbuffers(1, &depthRbo);     depthRbo = 0; }
    if (fbo)          { glDeleteFramebuffers(1, &fbo);           fbo = 0; }
    fbWidth  = 0;
    fbHeight = 0;
}

void GraphViewport::resize(int w, int h) {
    if (w == fbWidth && h == fbHeight) return;
    if (w <= 0 || h <= 0) return;

    deleteFramebuffer();
    createFramebuffer(w, h);
}

void GraphViewport::render() {
    if (!initialized || !shader || fbWidth <= 0 || fbHeight <= 0) return;

    // Save current GL state to avoid conflict with ImGui's rendering
    GLint prevFBO;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFBO);
    GLint prevViewport[4];
    glGetIntegerv(GL_VIEWPORT, prevViewport);

    // Bind FBO and set viewport
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, fbWidth, fbHeight);

    // Clear
    glClearColor(bgColor[0], bgColor[1], bgColor[2], 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render scene
    float aspectRatio = static_cast<float>(fbWidth) / static_cast<float>(fbHeight);
    shader->use();
    scene.render(*shader, aspectRatio);

    // Restore previous GL state
    glBindFramebuffer(GL_FRAMEBUFFER, prevFBO);
    glViewport(prevViewport[0], prevViewport[1],
               prevViewport[2], prevViewport[3]);
}

void GraphViewport::setBgColor(float r, float g, float b) {
    bgColor[0] = r;
    bgColor[1] = g;
    bgColor[2] = b;
}
