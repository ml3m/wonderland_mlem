#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H

#include "wonderlands.h"

typedef struct {
    GLuint program;
    char* name;
} Shader;

// Function prototypes
GLuint shader_load(const char* vertexPath, const char* fragmentPath);
GLuint shader_loadWithGeometry(const char* vertexPath, const char* geometryPath, const char* fragmentPath);
GLuint shader_loadCompute(const char* computePath);
void shader_use(GLuint program);
void shader_setInt(GLuint program, const char* name, int value);
void shader_setFloat(GLuint program, const char* name, float value);
void shader_setVec2(GLuint program, const char* name, float x, float y);
void shader_setVec3(GLuint program, const char* name, float x, float y, float z);
void shader_setVec4(GLuint program, const char* name, float x, float y, float z, float w);
void shader_setMat4(GLuint program, const char* name, const float* value);

#endif // SHADER_LOADER_H 