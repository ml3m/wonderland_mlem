#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include "wonderlands.h"

// Texture types
typedef enum {
    TEXTURE_DIFFUSE,
    TEXTURE_SPECULAR,
    TEXTURE_NORMAL,
    TEXTURE_HEIGHT,
    TEXTURE_AO,
    TEXTURE_ROUGHNESS,
    TEXTURE_METALLIC,
    TEXTURE_EMISSIVE
} TextureType;

typedef struct {
    GLuint id;
    TextureType type;
    char* path;
} Texture;

// Function prototypes
GLuint texture_load(const char* path, bool gammaCorrection);
GLuint texture_loadCubemap(const char* faces[6]);
void texture_bind(GLuint texture, GLenum unit);

#endif // TEXTURE_LOADER_H 