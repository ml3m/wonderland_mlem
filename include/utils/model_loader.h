#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include "wonderlands.h"

// Mesh structure
typedef struct {
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    unsigned int numVertices;
    unsigned int numIndices;
    bool hasNormals;
    bool hasTexCoords;
    bool hasTangents;
} Mesh;

// Model structure
typedef struct {
    Mesh* meshes;
    unsigned int numMeshes;
    char* name;
    char* path;
} Model;

// Function prototypes
Model* model_load(const char* path);
void model_cleanup(Model* model);
void model_render(Model* model, GLuint shader);
void model_renderMesh(Mesh* mesh, GLuint shader);

#endif // MODEL_LOADER_H 