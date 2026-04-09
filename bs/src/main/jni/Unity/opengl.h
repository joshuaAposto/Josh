
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <dlfcn.h>
#include "Substrate/SubstrateHook.h"
#include "Substrate/CydiaSubstrate.h"

static void *handle;
static const char* shaderName;
static bool enableWallhack, isRGB;
static bool enableWallhackW;
static bool nightmode;

static bool wh1;
static bool wh2 = true;
static bool wh3;

static bool rgb0;
static bool rgb1;
static bool rgb2;
static bool rgb3;

static bool arms;

static bool rgb4;
static bool rgb5;

static bool texFix;

static bool enableWireframe;
static bool enableGlow;
static bool enableShading;
static bool wallhack;
static bool fakelag;
static bool darkmode1;
static bool darkmode3;
static bool remove1;
static bool darkmode2;
static bool darkmodeW;
static bool darkmodeG;

static bool darkmode4;
static float r = 0;
static float g = 0;
static float b = 0;

static float armsR = 0;
static float armsG = 0;
static float armsB = 0;

static float nightvalR = 0;
static float nightvalG = 0;
static float nightvalB = 0;

static float r2 = 0;
static float g2 = 0;
static float b2 = 0;

static bool rS = false;
static bool gS = false;
static bool bS = false;

static bool rS2 = false;
static bool gS2 = false;
static bool bS2 = false;

static bool whSh = false;

static float a = 0;
static float rw = 0;
static float gw = 255;
static float bw = 0;
static float rg = 0;
static float gg = 0;
static float bg = 0;

static float widthw = 1;
static float widthg = 1;

static bool PulseT1;
static bool PulseT2;
static bool PulseW;
static bool PulseG;
static bool PulseA;
static int red = 255, green = 0, blue = 255, rgb = 1, pulsesw = 0, pulsevalue = 0;
static bool rF = true, gF, bF = true, rF_, gF_, bF_;

void setShader(const char* s) {
    handle = dlopen("libGLESv2.so", RTLD_LAZY);
    shaderName = s;
}

const char* getShader() {
    return shaderName;
}

void performRGBChange() {
    switch (rgb) {
        case 0: {
            if (gF) {
                green--;
                if (!green) {
                    gF = !gF;
                    rF_ = !rF_;
                }
                break;
            }

            if (red < 255 && rF_) red++;
            else {
                rgb++;
                red = 255;
                rF = !rF;
                rF_ = !rF_;
            }
            break;
        }
        case 1: {
            if (bF) {
                blue--;
                if (!blue) {
                    bF = !bF;
                    gF_ = !gF_;
                }
                break;
            }

            if (green < 255 && gF_) green++;
            else {
                rgb++;
                green = 255;
                gF = !gF;
                gF_ = !gF_;
            }
            break;
        }
        case 2: {
            if (rF) {
                red--;
                if (!red) {
                    rF = !rF;
                    bF_ = !bF_;
                }
                break;
            }

            if (blue < 255 && bF_) blue++;
            else {
                rgb = 0;
                blue = 255;
                bF = !bF;
                bF_ = !bF_;
            }
        }
    }
}
void perfPulse() {
    switch (pulsesw) {
        case 0: {
            if (pulsevalue < 100)  pulsevalue++;
            else pulsesw++;
            break;
        }
        case 1: {
            if (pulsevalue > 0)  pulsevalue--;
            else pulsesw--;
            break;
        }
    }
}

bool offhud = false;

bool armsChams;
bool worldChams;
bool opacityChams;
bool texturedChams;
bool wireframeChams;
bool wireframeWallhackChams;
bool shadingChams;
bool shadingWallhackChams;
bool outlineChams;
bool outlineWallhackChams;


float wireframeWidthChams;
float outlineWidthChams;


ImVec4 armsChamsColor = ImVec4(1.0f, 1.0f, 1.0f, 0.3f);
bool armsChamsRainbow;

ImVec4 worldChamsColor = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
bool worldChamsRainbow;

ImVec4 visibleTexturedChamsColor = ImVec4(0.0f, 1.0f, 0.0f, 0.3f);
bool visibleTexturedChamsRainbow;
bool visibleTexturedChamsPulse;
bool visibleTexturedChamsDark;

ImVec4 invisibleTexturedChamsColor = ImVec4(1.0f, 0.0f, 0.0f, 0.3f);
bool invisibleTexturedChamsRainbow;
bool invisibleTexturedChamsPulse;
bool invisibleTexturedChamsDark;

ImVec4 wireframeChamsColor = ImVec4(1.0f, 1.0f, 1.0f, 0.3f);
bool wireframeChamsRainbow;
bool wireframeChamsPulse;
bool wireframeChamsDark;

ImVec4 shadingChamsColor = ImVec4(1.0f, 1.0f, 1.0f, 0.3f);

ImVec4 outlineChamsColor = ImVec4(1.0f, 1.0f, 1.0f, 0.3f);
bool outlineChamsRainbow;
bool outlineChamsPulse;
bool outlineChamsDark;

ImVec4 glowChamsColor = ImVec4(1.0f, 1.0f, 1.0f, 0.3f);
bool glowChamsRainbow;
bool glowChamsPulse;
bool glowChamsDark;
bool glowChams;

int (*old_glGetUniformLocation)(GLuint, const GLchar *);
GLint new_glGetUniformLocation(GLuint program, const GLchar *name) {
    return old_glGetUniformLocation(program, name);
}

bool isCurrentShader(const char *shader) {
    GLint currProgram;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currProgram);
    return old_glGetUniformLocation(currProgram, shader) != -1;
}

GL_APICALL void GL_APIENTRY (*old_glDrawElements)(...);
GL_APICALL void GL_APIENTRY new_glDrawElements(GLenum mode, GLsizei count, GLenum type, const void *indices) {
    if (!texFix) {

        if (!offhud && !shadingChams && !texturedChams && !wireframeChams && !outlineChams &&
            !nightmode) {

            old_glDrawElements(mode, count, type, indices);
            return;
        }

        GLint currProgram;
        glGetIntegerv(GL_CURRENT_PROGRAM, &currProgram);
        GLint id = glGetUniformLocation(currProgram, "_BumpMap");
        GLint id3 = glGetUniformLocation(currProgram, "_Glossiness");
        GLint id5 = glGetUniformLocation(currProgram, "_Metallic");
        GLint id4 = glGetUniformLocation(currProgram, "_Color");
        GLint id2 = glGetUniformLocation(currProgram, "_MainTex");

        /* if (id4 != -1 && offhud) {
              glEnable(GL_BLEND);
              glBlendColor(0, 0, 0, 1);
              glColorMask(1, 1, 1, 1);
              glBlendFuncSeparate(GL_CONSTANT_COLOR, GL_CONSTANT_ALPHA, GL_ONE, GL_ZERO);
              old_glDrawElements(mode, count, type, indices);
              return;
          }*/
        if (mode != GL_TRIANGLES) return;
        //if(id != -1){
        if ((id != -1)/*&&(id4 == -1) || (id3 != -1 && id5 != -1)*/) {
            if (!opacityChams) old_glDrawElements(mode, count, type, indices);
            if (shadingChams) {
                glDepthRangef((shadingWallhackChams + 1) / 2, 1.5 - (shadingWallhackChams + 1) / 2);
                glEnable(GL_BLEND);
                glBlendColor(0, 0, 0, 0);
                glColorMask(!shadingChamsColor.x, !shadingChamsColor.y, !shadingChamsColor.z, 1);
                glBlendFuncSeparate(GL_CONSTANT_COLOR, GL_CONSTANT_ALPHA, GL_ONE, GL_ZERO);
                old_glDrawElements(GL_TRIANGLES, count, type, indices);
            }
            if (texturedChams) {
                glEnable(GL_CULL_FACE);
                glFrontFace(GL_CW);
                glBlendColor((visibleTexturedChamsRainbow ? float(red) / 255
                                                          : visibleTexturedChamsColor.x) *
                             (visibleTexturedChamsPulse ? float(pulsevalue) / 100 : 1),
                             (visibleTexturedChamsRainbow ? float(green) / 255
                                                          : visibleTexturedChamsColor.y) *
                             (visibleTexturedChamsPulse ? float(pulsevalue) / 100 : 1),
                             (visibleTexturedChamsRainbow ? float(blue) / 255
                                                          : visibleTexturedChamsColor.z) *
                             (visibleTexturedChamsPulse ? float(pulsevalue) / 100 : 1),
                             visibleTexturedChamsColor.w);
                glColorMask(1, 1, 1, 1);
                glEnable(GL_BLEND);
                glEnable(GL_STENCIL_TEST);
                glStencilMask(0);
                glDepthMask(true);
                glDepthFunc(GL_EQUAL);
                glBlendFuncSeparate(GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_ALPHA, GL_ONE, GL_ONE);
                old_glDrawElements(GL_TRIANGLES, count, type, indices);
                glEnable(GL_BLEND);
                glDepthFunc(GL_NOTEQUAL);
                glBlendFuncSeparate(GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_ALPHA, GL_ONE, GL_ONE);
                glBlendColor((invisibleTexturedChamsRainbow ? float(red) / 255
                                                            : invisibleTexturedChamsColor.x) *
                             (invisibleTexturedChamsPulse ? float(pulsevalue) / 100 : 1),
                             (invisibleTexturedChamsRainbow ? float(green) / 255
                                                            : invisibleTexturedChamsColor.y) *
                             (invisibleTexturedChamsPulse ? float(pulsevalue) / 100 : 1),
                             (invisibleTexturedChamsRainbow ? float(blue) / 255
                                                            : invisibleTexturedChamsColor.z) *
                             (invisibleTexturedChamsPulse ? float(pulsevalue) / 100 : 1),
                             invisibleTexturedChamsColor.w);
                old_glDrawElements(GL_TRIANGLES, count, type, indices);
                glDepthMask(true);
                glDepthFunc(GL_LESS);
            }
            if (glowChams) {
                glEnable(GL_BLEND);
                glBlendColor((glowChamsRainbow ? float(red) / 255 : glowChamsColor.x) *
                             (glowChamsPulse ? float(pulsevalue) / 100 : 1),
                             (glowChamsRainbow ? float(green) / 255 : glowChamsColor.y) *
                             (glowChamsPulse ? float(pulsevalue) / 100 : 1),
                             (glowChamsRainbow ? float(blue) / 255 : glowChamsColor.z) *
                             (glowChamsPulse ? float(pulsevalue) / 100 : 1),
                             glowChamsColor.w);
                glColorMask(1, 1, 1, 1);
                glEnable(GL_BLEND);
                glBlendFuncSeparate(GL_CONSTANT_COLOR, GL_CONSTANT_ALPHA, GL_ONE, GL_ZERO);
                glLineWidth(6);
                glDepthRangef(0.5, 1);
                old_glDrawElements(GL_LINES, count, type, indices);
                glBlendColor(1, 1, 1, 1);
                glDepthRangef(1, 0.5);
                old_glDrawElements(GL_TRIANGLES, count, type, indices);
            }
            if (wireframeChams) {
                glDepthRangef((wireframeWallhackChams + 1) / 2,
                              1.5 - (wireframeWallhackChams + 1) / 2);
                glEnable(GL_BLEND);
                glColorMask(1, 1, 1, 1);
                glBlendFuncSeparate(GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_ALPHA, GL_ONE, GL_ONE);
                glBlendColor((wireframeChamsRainbow ? float(red) / 255 : wireframeChamsColor.x) *
                             (wireframeChamsPulse ? float(pulsevalue) / 100 : 1),
                             (wireframeChamsRainbow ? float(green) / 255 : wireframeChamsColor.y) *
                             (wireframeChamsPulse ? float(pulsevalue) / 100 : 1),
                             (wireframeChamsRainbow ? float(blue) / 255 : wireframeChamsColor.z) *
                             (wireframeChamsPulse ? float(pulsevalue) / 100 : 1),
                             wireframeChamsColor.w);
                glLineWidth(wireframeWidthChams);
                old_glDrawElements(GL_LINES, count, type, indices);
            }
            if (outlineChams) {
                glDepthRangef(1, 0);
                glEnable(GL_BLEND);
                glColorMask(1, 1, 1, 1);
                glBlendFuncSeparate(GL_CONSTANT_COLOR, GL_CONSTANT_ALPHA, GL_ONE, GL_ZERO);
                glBlendColor(0, 0, 0, 1);
                old_glDrawElements(GL_TRIANGLES, count, type, indices);
                glBlendFuncSeparate(GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_ALPHA, GL_ONE, GL_ONE);
                glBlendColor((outlineChamsRainbow ? float(red) / 255 : outlineChamsColor.x) *
                             (outlineChamsPulse ? float(pulsevalue) / 100 : 1),
                             (outlineChamsRainbow ? float(green) / 255 : outlineChamsColor.y) *
                             (outlineChamsPulse ? float(pulsevalue) / 100 : 1),
                             (outlineChamsRainbow ? float(blue) / 255 : outlineChamsColor.z) *
                             (outlineChamsPulse ? float(pulsevalue) / 100 : 1),
                             outlineChamsColor.w);
                glDepthRangef((outlineWallhackChams + 1) / 2, 1.5 - (outlineWallhackChams + 1) / 2);
                glLineWidth(outlineWidthChams);
                old_glDrawElements(GL_LINES, count, type, indices);
                glDepthRangef(0.5, 1);
                glColorMask(GL_ONE, GL_ONE, GL_ONE, GL_ONE);
                glDisable(GL_BLEND);
            }
            glDepthRangef(0.5, 1);
            glColorMask(GL_ONE, GL_ONE, GL_ONE, GL_ONE);
            glDisable(GL_BLEND);
        } else old_glDrawElements(mode, count, type, indices);
        if (id2 != -1 && id == -1 && id4 == -1 && id3 == -1 && id5 == -1) {
            if (nightmode) {
                glBlendColor((worldChamsRainbow ? float(red) / 255 : worldChamsColor.x),
                             (worldChamsRainbow ? float(green) / 255 : worldChamsColor.y),
                             (worldChamsRainbow ? float(blue) / 255 : worldChamsColor.z),
                             worldChamsColor.w);
                glColorMask(1, 1, 1, 1);
                glEnable(GL_BLEND);
                glBlendFuncSeparate(GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_ALPHA, GL_ONE, GL_ONE);
                old_glDrawElements(GL_TRIANGLES, count, type, indices);
            }
        }

    } else {

        if (!offhud && !shadingChams && !texturedChams && !wireframeChams && !outlineChams &&
            !nightmode) {

            old_glDrawElements(mode, count, type, indices);
            return;
        }
        GLint currProgram;
        glGetIntegerv(GL_CURRENT_PROGRAM, &currProgram);
        GLint id = glGetUniformLocation(currProgram, "unity_SHC");
        GLint id3 = glGetUniformLocation(currProgram, "_Glossiness");
        GLint id5 = glGetUniformLocation(currProgram, "_Metallic");
        GLint id4 = glGetUniformLocation(currProgram, "_Color");
        GLint id2 = glGetUniformLocation(currProgram, "_MainTex");

        if (id4 != -1 && offhud) {
            glEnable(GL_BLEND);
            glBlendColor(0, 0, 0, 1);
            glColorMask(1, 1, 1, 1);
            glBlendFuncSeparate(GL_CONSTANT_COLOR, GL_CONSTANT_ALPHA, GL_ONE, GL_ZERO);
            old_glDrawElements(mode, count, type, indices);
            return;
        }
        if (mode != GL_TRIANGLES) return;
        if ((id != -1 && id4 == -1) || (id3 != -1 && id5 != -1)) {
            if (!opacityChams) old_glDrawElements(mode, count, type, indices);
            if (shadingChams) {
                glDepthRangef((shadingWallhackChams + 1) / 2, 1.5 - (shadingWallhackChams + 1) / 2);
                glEnable(GL_BLEND);
                glBlendColor(0, 0, 0, 0);
                glColorMask(!shadingChamsColor.x, !shadingChamsColor.y, !shadingChamsColor.z, 1);
                glBlendFuncSeparate(GL_CONSTANT_COLOR, GL_CONSTANT_ALPHA, GL_ONE, GL_ZERO);
                old_glDrawElements(GL_TRIANGLES, count, type, indices);
            }
            if (texturedChams) {
                glEnable(GL_CULL_FACE);
                glFrontFace(GL_CW);
                glBlendColor((visibleTexturedChamsRainbow ? float(red) / 255
                                                          : visibleTexturedChamsColor.x) *
                             (visibleTexturedChamsPulse ? float(pulsevalue) / 100 : 1),
                             (visibleTexturedChamsRainbow ? float(green) / 255
                                                          : visibleTexturedChamsColor.y) *
                             (visibleTexturedChamsPulse ? float(pulsevalue) / 100 : 1),
                             (visibleTexturedChamsRainbow ? float(blue) / 255
                                                          : visibleTexturedChamsColor.z) *
                             (visibleTexturedChamsPulse ? float(pulsevalue) / 100 : 1),
                             visibleTexturedChamsColor.w);
                glColorMask(1, 1, 1, 1);
                glEnable(GL_BLEND);
                glEnable(GL_STENCIL_TEST);
                glStencilMask(0);
                glDepthMask(true);
                glDepthFunc(GL_EQUAL);
                glBlendFuncSeparate(GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_ALPHA, GL_ONE, GL_ONE);
                old_glDrawElements(GL_TRIANGLES, count, type, indices);
                glEnable(GL_BLEND);
                glDepthFunc(GL_NOTEQUAL);
                glBlendFuncSeparate(GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_ALPHA, GL_ONE, GL_ONE);
                glBlendColor((invisibleTexturedChamsRainbow ? float(red) / 255
                                                            : invisibleTexturedChamsColor.x) *
                             (invisibleTexturedChamsPulse ? float(pulsevalue) / 100 : 1),
                             (invisibleTexturedChamsRainbow ? float(green) / 255
                                                            : invisibleTexturedChamsColor.y) *
                             (invisibleTexturedChamsPulse ? float(pulsevalue) / 100 : 1),
                             (invisibleTexturedChamsRainbow ? float(blue) / 255
                                                            : invisibleTexturedChamsColor.z) *
                             (invisibleTexturedChamsPulse ? float(pulsevalue) / 100 : 1),
                             invisibleTexturedChamsColor.w);
                old_glDrawElements(GL_TRIANGLES, count, type, indices);
                glDepthMask(true);
                glDepthFunc(GL_LESS);
            }
            if (glowChams) {
                glEnable(GL_BLEND);
                glBlendColor((glowChamsRainbow ? float(red) / 255 : glowChamsColor.x) *
                             (glowChamsPulse ? float(pulsevalue) / 100 : 1),
                             (glowChamsRainbow ? float(green) / 255 : glowChamsColor.y) *
                             (glowChamsPulse ? float(pulsevalue) / 100 : 1),
                             (glowChamsRainbow ? float(blue) / 255 : glowChamsColor.z) *
                             (glowChamsPulse ? float(pulsevalue) / 100 : 1),
                             glowChamsColor.w);
                glColorMask(1, 1, 1, 1);
                glEnable(GL_BLEND);
                glBlendFuncSeparate(GL_CONSTANT_COLOR, GL_CONSTANT_ALPHA, GL_ONE, GL_ZERO);
                glLineWidth(6);
                glDepthRangef(0.5, 1);
                old_glDrawElements(GL_LINES, count, type, indices);
                glBlendColor(1, 1, 1, 1);
                glDepthRangef(1, 0.5);
                old_glDrawElements(GL_TRIANGLES, count, type, indices);
            }
            if (wireframeChams) {
                glDepthRangef((wireframeWallhackChams + 1) / 2,
                              1.5 - (wireframeWallhackChams + 1) / 2);
                glEnable(GL_BLEND);
                glColorMask(1, 1, 1, 1);
                glBlendFuncSeparate(GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_ALPHA, GL_ONE, GL_ONE);
                glBlendColor((wireframeChamsRainbow ? float(red) / 255 : wireframeChamsColor.x) *
                             (wireframeChamsPulse ? float(pulsevalue) / 100 : 1),
                             (wireframeChamsRainbow ? float(green) / 255 : wireframeChamsColor.y) *
                             (wireframeChamsPulse ? float(pulsevalue) / 100 : 1),
                             (wireframeChamsRainbow ? float(blue) / 255 : wireframeChamsColor.z) *
                             (wireframeChamsPulse ? float(pulsevalue) / 100 : 1),
                             wireframeChamsColor.w);
                glLineWidth(wireframeWidthChams);
                old_glDrawElements(GL_LINES, count, type, indices);
            }
            if (outlineChams) {
                glDepthRangef(1, 0);
                glEnable(GL_BLEND);
                glColorMask(1, 1, 1, 1);
                glBlendFuncSeparate(GL_CONSTANT_COLOR, GL_CONSTANT_ALPHA, GL_ONE, GL_ZERO);
                glBlendColor(0, 0, 0, 1);
                old_glDrawElements(GL_TRIANGLES, count, type, indices);
                glBlendFuncSeparate(GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_ALPHA, GL_ONE, GL_ONE);
                glBlendColor((outlineChamsRainbow ? float(red) / 255 : outlineChamsColor.x) *
                             (outlineChamsPulse ? float(pulsevalue) / 100 : 1),
                             (outlineChamsRainbow ? float(green) / 255 : outlineChamsColor.y) *
                             (outlineChamsPulse ? float(pulsevalue) / 100 : 1),
                             (outlineChamsRainbow ? float(blue) / 255 : outlineChamsColor.z) *
                             (outlineChamsPulse ? float(pulsevalue) / 100 : 1),
                             outlineChamsColor.w);
                glDepthRangef((outlineWallhackChams + 1) / 2, 1.5 - (outlineWallhackChams + 1) / 2);
                glLineWidth(outlineWidthChams);
                old_glDrawElements(GL_LINES, count, type, indices);
                glDepthRangef(0.5, 1);
                glColorMask(GL_ONE, GL_ONE, GL_ONE, GL_ONE);
                glDisable(GL_BLEND);
            }
            glDepthRangef(0.5, 1);
            glColorMask(GL_ONE, GL_ONE, GL_ONE, GL_ONE);
            glDisable(GL_BLEND);
        } else old_glDrawElements(mode, count, type, indices);
        if (id2 != -1 && id == -1 && id4 == -1 && id5 == -1 && id3 == -1) {
            if (nightmode) {
                glBlendColor((worldChamsRainbow ? float(red) / 255 : worldChamsColor.x),
                             (worldChamsRainbow ? float(green) / 255 : worldChamsColor.y),
                             (worldChamsRainbow ? float(blue) / 255 : worldChamsColor.z),
                             worldChamsColor.w);
                glColorMask(1, 1, 1, 1);
                glEnable(GL_BLEND);
                glBlendFuncSeparate(GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_ALPHA, GL_ONE, GL_ONE);
                old_glDrawElements(GL_TRIANGLES, count, type, indices);
            }
        }
        /*
        if (id2 != -1 && id == -1 && id4 == -1) {
            if (nightmode) {
                glBlendColor((worldChamsRainbow ? float(red)/255 : worldChamsColor.x),
                             (worldChamsRainbow ? float(green)/255 : worldChamsColor.y),
                             (worldChamsRainbow ? float(blue)/255 : worldChamsColor.z), worldChamsColor.w);
                glColorMask(1, 1, 1, 1);
                glEnable(GL_BLEND);
                glBlendFuncSeparate(GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_ALPHA, GL_ONE, GL_ONE);
                old_glDrawElements(GL_TRIANGLES, count, type, indices);
            }
        }*/

    }
}

bool mlovinit(){
    handle = NULL;
    handle = dlopen("libGLESv2.so", RTLD_LAZY);
    if(!handle){
        //LOGE("Cannot open library: %s", dlerror());
        return false;
    }
    return true;
}

void LogShaders(){
    auto p_glGetUniformLocation = (const void*(*)(...))dlsym(handle, "glGetUniformLocation");
    const char *dlsym_error = dlerror();
    if(dlsym_error){
        //LOGE("Cannot load symbol 'glGetUniformLocation': %s", dlsym_error);
        return;
    }else{
        MSHookFunction(reinterpret_cast<void*>(p_glGetUniformLocation), reinterpret_cast<void*>(new_glGetUniformLocation), reinterpret_cast<void**>(&old_glGetUniformLocation));
    }
}

void Wallhack(){
    MSHookFunction((void*)glDrawElements, (void*)new_glDrawElements,(void**)&old_glDrawElements);
}

