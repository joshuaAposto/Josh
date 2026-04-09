#ifndef ZEFF_CHAMS
#define ZEFF_CHAMS
#include <dlfcn.h>
#include <array>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <string>
#include <cstring>
#include "openGLdefine.h"
#include "Dobby/dobby.h"
#include "include/obfuscate.h"
#include "include/hpp/imgui.h"

static int w_chams = 0;
static std::vector<std::string> shaders;
static std::string mineShader = "tNormalMapSampler";

static int chamsint = 0;

static ImVec4 inWallColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
static ImVec4 visibleColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);

static void (*p_glGetIntegerv)(unsigned int, int *);
static void (*p_glGetBooleanv)(unsigned int, unsigned char *);
static void (*p_glDepthRangef)(float, float);
static void (*p_glBlendColor)(float, float, float, float);
static void (*p_glEnable)(unsigned int);
static void (*p_glLineWidth)(float);
static void (*p_glBlendFunc)(unsigned int, unsigned int);
static void (*p_glDisable)(unsigned int);
static void (*p_glCullFace)(unsigned int);

static int (*old_glGetUniformLocation)(unsigned int, const char *);
static int new_glGetUniformLocation(unsigned int program, const char *name) {
    bool isExist = false;
    std::string thisShader = {name};
    for (std::string shader : shaders) {
        if (shader.c_str() == thisShader) isExist = true;
    }
    if (!isExist) shaders.push_back({name});
    return old_glGetUniformLocation(program, name);
}

static void (*old_glDrawElements)(unsigned int, int, unsigned int, const void *);
static void new_glDrawElements(unsigned int mode, int count, unsigned int type, const void *indices) {
    if (mode != GL_TRIANGLES) return old_glDrawElements(mode, count, type, indices);
    if (count < 500 || count == 4789 || count == 4790 || count == 4791 || count == 5815 ||
    count == 6838 || count == 6839 || count == 11373 || count == 912 || count == 1527 ||
    count == 5835) return old_glDrawElements(mode, count, type, indices);

    unsigned char blend;
    p_glGetBooleanv(GL_BLEND, &blend);
    if (blend) return old_glDrawElements(mode, count, type, indices);

    unsigned char depthTest;
    p_glGetBooleanv(GL_DEPTH_TEST, &depthTest);
    if (!depthTest) return old_glDrawElements(mode, count, type, indices);

    int currProgram;
    p_glGetIntegerv(GL_CURRENT_PROGRAM, &currProgram);
    const char* currentShader = mineShader.c_str();
    int shader = old_glGetUniformLocation(currProgram, currentShader);
    if (shader == -1) return old_glDrawElements(mode, count, type, indices);

    {
        if (chamsint == 0) return old_glDrawElements(mode, count, type, indices);

        if (chamsint == 1) {
            p_glDepthRangef(1, 0.5);
            old_glDrawElements(mode, count, type, indices);
        }

        if (chamsint == 2) {
            old_glDrawElements(mode, count, type, indices);
            p_glDepthRangef(1, 0.5);
            p_glBlendColor(visibleColor.x, visibleColor.y, visibleColor.z, 1);
            p_glEnable(GL_BLEND);
            p_glBlendFunc(GL_ONE, GL_CONSTANT_COLOR);
            old_glDrawElements(mode, count, type, indices);
        }

        if (chamsint == 3) {
            old_glDrawElements(mode, count, type, indices);
            p_glDepthRangef(1, 0.5);
            p_glBlendColor(visibleColor.x, visibleColor.y, visibleColor.z, 1);
            p_glEnable(GL_BLEND);
            p_glBlendFunc(GL_CONSTANT_COLOR, GL_CONSTANT_ALPHA);
            p_glLineWidth(1);
            old_glDrawElements(GL_LINES, count, type, indices);
        }

        if (chamsint == 4) {
            old_glDrawElements(mode, count, type, indices);
            p_glDepthRangef(1, 0);
            p_glLineWidth(10);
            p_glEnable(GL_BLEND);
            p_glBlendFunc(GL_CONSTANT_COLOR, GL_CONSTANT_ALPHA);
            p_glBlendColor(0, 0, 0, 1);
            old_glDrawElements(mode, count, type, indices);
            p_glDepthRangef(1, 0.5);
            p_glBlendColor(visibleColor.x, visibleColor.y, visibleColor.z, 1);
            old_glDrawElements(GL_LINES, count, type, indices);
        }

        if (chamsint == 5) {
            p_glEnable(GL_BLEND);
            p_glBlendFunc(GL_CONSTANT_COLOR, GL_CONSTANT_ALPHA);
            p_glLineWidth(10);
            p_glBlendColor(visibleColor.x, visibleColor.y, visibleColor.z, 1);
            p_glDepthRangef(0.5, 1);
            old_glDrawElements(GL_LINES, count, type, indices);
            p_glBlendFunc(GL_ONE_MINUS_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_ALPHA);
            p_glBlendColor(0, 0, 0, 1);
            p_glDepthRangef(1, 0.5);
            old_glDrawElements(mode, count, type, indices);
        }

        if (chamsint == 6) {
            p_glDisable(GL_DEPTH_TEST);
            p_glEnable(GL_BLEND);
            p_glBlendFunc(GL_ONE, GL_CONSTANT_COLOR);
            p_glBlendColor(inWallColor.x, inWallColor.y, inWallColor.z, 1);
            old_glDrawElements(mode, count, type, indices);
            p_glEnable(GL_DEPTH_TEST);
            p_glDepthRangef(0.5, 1);
            p_glBlendColor(visibleColor.x, visibleColor.y, visibleColor.z, 1);
            old_glDrawElements(mode, count, type, indices);
        }

        if (chamsint == 7) {
            p_glDisable(GL_DEPTH_TEST);
            p_glEnable(GL_BLEND);
            p_glBlendFunc(GL_ONE, GL_CONSTANT_COLOR);
            p_glBlendColor(inWallColor.x, inWallColor.y, inWallColor.z, 1);
            old_glDrawElements(mode, count, type, indices);
            p_glEnable(GL_DEPTH_TEST);
            p_glDepthRangef(0.5, 1);
            p_glBlendColor(visibleColor.x, visibleColor.y, visibleColor.z, 1);
            p_glLineWidth(10);
            old_glDrawElements(GL_LINES, count, type, indices);
            p_glBlendFunc(GL_ONE_MINUS_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_ALPHA);
            p_glDepthRangef(0.49, 1);
            p_glBlendColor(0, 0, 0, 1);
            old_glDrawElements(mode, count, type, indices);
        }
    }
    p_glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    p_glLineWidth(1);
    p_glBlendColor(0, 0, 0, 1);
    p_glDepthRangef(0, 1);
    return p_glDisable(GL_BLEND);
}

static void initChams() {
    void* libGLES = dlopen(oxorany("libGLESv3.so"), 4);
    if (!libGLES) return;
    p_glGetIntegerv  = (void (*)(unsigned int, int *))        dlsym(libGLES, oxorany("glGetIntegerv"));
    p_glGetBooleanv  = (void (*)(unsigned int, unsigned char*))dlsym(libGLES, oxorany("glGetBooleanv"));
    p_glDepthRangef  = (void (*)(float, float))               dlsym(libGLES, oxorany("glDepthRangef"));
    p_glBlendColor   = (void (*)(float, float, float, float)) dlsym(libGLES, oxorany("glBlendColor"));
    p_glEnable       = (void (*)(unsigned int))               dlsym(libGLES, oxorany("glEnable"));
    p_glLineWidth    = (void (*)(float))                      dlsym(libGLES, oxorany("glLineWidth"));
    p_glBlendFunc    = (void (*)(unsigned int, unsigned int)) dlsym(libGLES, oxorany("glBlendFunc"));
    p_glDisable      = (void (*)(unsigned int))               dlsym(libGLES, oxorany("glDisable"));
    p_glCullFace     = (void (*)(unsigned int))               dlsym(libGLES, oxorany("glCullFace"));
    DobbyHook((void*)dlsym(libGLES, oxorany("glGetUniformLocation")), (void*)new_glGetUniformLocation, (void**)&old_glGetUniformLocation);
    DobbyHook((void*)dlsym(libGLES, oxorany("glDrawElements")),       (void*)new_glDrawElements,       (void**)&old_glDrawElements);
}

#endif
