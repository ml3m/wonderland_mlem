#ifndef SKYBOX_H
#define SKYBOX_H

#include "wonderlands.h"

// Skybox structure
typedef struct {
    // Mesh data
    GLuint vao;
    GLuint vbo;
    
    // Textures
    GLuint dayCubemap;
    GLuint nightCubemap;
    GLuint sunsetCubemap;
    
    // Cloud settings
    GLuint cloudTexture;
    GLuint cloudNormalMap;
    float cloudDensity;
    float cloudHeight;
    float cloudSpeed;
    
    // Sun/moon settings
    vec3 sunDirection;
    vec3 sunColor;
    float sunSize;
    vec3 moonDirection;
    vec3 moonColor;
    float moonSize;
    GLuint moonTexture;
    
    // Atmospheric scattering
    bool useScattering;
    vec3 rayleighScattering;
    float mieScattering;
    float scatteringStrength;
    
    // Stars
    GLuint starTexture;
    float starBrightness;
    float twinkleSpeed;
} Skybox;

// Function prototypes
void skybox_init(Skybox* skybox);
void skybox_cleanup(Skybox* skybox);
void skybox_setupMesh(Skybox* skybox);
void skybox_loadTextures(Skybox* skybox);
void skybox_update(Skybox* skybox, float timeOfDay, WeatherType weather);
void skybox_render(Skybox* skybox, Camera* camera, GLuint shader);
void skybox_calculateSunPosition(Skybox* skybox, float timeOfDay);
void skybox_calculateMoonPosition(Skybox* skybox, float timeOfDay);
void skybox_getAmbientLight(Skybox* skybox, float timeOfDay, WeatherType weather, vec3 ambient);
void skybox_getSunLight(Skybox* skybox, float timeOfDay, WeatherType weather, vec3 direction, vec3 color, float* intensity);

#endif // SKYBOX_H 