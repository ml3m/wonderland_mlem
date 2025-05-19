#ifndef WATER_H
#define WATER_H

#include "wonderlands.h"

// Water system structure
typedef struct {
    // Mesh data
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    int indexCount;
    
    // Grid properties
    int gridSize;
    float cellSize;
    float height;
    
    // Rendering properties
    GLuint reflectionTexture;
    GLuint refractionTexture;
    GLuint refractionDepthTexture;
    GLuint dudvMap;
    GLuint normalMap;
    
    // Reflection/refraction framebuffers
    GLuint reflectionFBO;
    GLuint refractionFBO;
    
    // Wave properties
    float waveHeight;
    float waveSpeed;
    float waveFrequency;
    
    // Flow simulation
    GLuint flowMap;
    GLuint obstacleMap;
    GLuint velocityMap;
    GLuint pressureMap;
    GLuint divergenceMap;
    
    // Flow simulation shaders
    GLuint advectionShader;
    GLuint pressureShader;
    GLuint divergenceShader;
    GLuint gradientShader;
    GLuint boundaryShader;
    
    // Animation
    float moveFactor;
    
    // Render quality
    int reflectionWidth;
    int reflectionHeight;
    int refractionWidth;
    int refractionHeight;
    
    // Ripple effect
    bool rippleEnabled;
    vec2* ripplePositions;
    float* rippleStrengths;
    float* rippleTimes;
    int rippleCount;
    int maxRipples;
} Water;

// Function prototypes
void water_init(Water* water, int gridSize, float cellSize, float height);
void water_cleanup(Water* water);
void water_setupMesh(Water* water);
void water_setupFramebuffers(Water* water);
void water_setupFlowMaps(Water* water);
void water_setupTextures(Water* water);
void water_update(Water* water, float deltaTime);
void water_addRipple(Water* water, float x, float z, float strength);
void water_updateRipples(Water* water, float deltaTime);
void water_simulateFlow(Water* water, float deltaTime);
void water_render(Water* water, Camera* camera, GLuint shader);
void water_bindReflectionFramebuffer(Water* water);
void water_bindRefractionFramebuffer(Water* water);
void water_unbindFramebuffer();
void water_bindTextures(Water* water, GLuint shader);

#endif // WATER_H 