#ifndef RENDERER_H
#define RENDERER_H

#include "wonderlands.h"

// Forward declarations
typedef struct SceneManager SceneManager;
typedef struct Camera Camera;

// Renderer configuration
typedef struct {
    // General settings
    bool enableShadows;
    bool enableSSAO;
    bool enableFXAA;
    bool enableBloom;
    bool enableDoF;
    bool enableGodRays;
    bool showWireframe;
    
    // Shadow settings
    int shadowMapResolution;
    float shadowBias;
    
    // Post-processing settings
    float exposureValue;
    float bloomThreshold;
    float bloomIntensity;
    float dofFocalDistance;
    float dofFocalRange;
    
    // Framebuffers
    GLuint gBuffer;
    GLuint gPosition;
    GLuint gNormal;
    GLuint gAlbedo;
    GLuint gMaterial;
    GLuint ssaoBuffer;
    GLuint ssaoBlurBuffer;
    GLuint hdrBuffer;
    GLuint pingpongBuffers[2];
    
    // Render targets
    GLuint depthRenderBuffer;
    GLuint colorAttachments[4];
    GLuint ssaoColorBuffer;
    GLuint ssaoBlurColorBuffer;
    GLuint hdrColorBuffer;
    GLuint pingpongColorBuffers[2];
    
    // Shaders
    GLuint gBufferShader;
    GLuint lightingShader;
    GLuint ssaoShader;
    GLuint ssaoBlurShader;
    GLuint shadowMapShader;
    GLuint skyboxShader;
    GLuint terrainShader;
    GLuint waterShader;
    GLuint vegetationShader;
    GLuint instancedShader;
    GLuint particleShader;
    GLuint postProcessShader;
    GLuint blurShader;
    GLuint compositShader;
    
    // Screen-space quad
    GLuint quadVAO;
    GLuint quadVBO;
    
    // SSAO
    GLuint ssaoKernelSize;
    vec3* ssaoKernel;
    GLuint ssaoNoiseTexture;
} Renderer;

// Function prototypes
void renderer_init(Renderer* renderer);
void renderer_cleanup(Renderer* renderer);
void renderer_render(Renderer* renderer, SceneManager* scene, Camera* camera, float timeOfDay, WeatherType weather);
void renderer_setupFramebuffers(Renderer* renderer);
void renderer_setupShaders(Renderer* renderer);
void renderer_setupQuad(Renderer* renderer);
void renderer_setupSSAO(Renderer* renderer);
void renderer_renderShadowMaps(Renderer* renderer, SceneManager* scene);
void renderer_geometryPass(Renderer* renderer, SceneManager* scene, Camera* camera);
void renderer_ssaoPass(Renderer* renderer, Camera* camera);
void renderer_lightingPass(Renderer* renderer, SceneManager* scene, Camera* camera, float timeOfDay);
void renderer_transparencyPass(Renderer* renderer, SceneManager* scene, Camera* camera, float timeOfDay);
void renderer_postProcessPass(Renderer* renderer);
void renderer_renderScene(Renderer* renderer, SceneManager* scene, Camera* camera, bool depthOnly);
void renderer_renderTerrain(Renderer* renderer, Terrain* terrain, Camera* camera, bool depthOnly);
void renderer_renderWater(Renderer* renderer, Water* water, Camera* camera);
void renderer_renderSkybox(Renderer* renderer, Skybox* skybox, Camera* camera);
void renderer_renderObject(Renderer* renderer, Object* object, Camera* camera, bool depthOnly);
void renderer_renderInstancedObjects(Renderer* renderer, Object* objects, size_t count, Camera* camera, bool depthOnly);
void renderer_renderParticles(Renderer* renderer, ParticleSystem* particles, Camera* camera);
void renderer_pick(Renderer* renderer, SceneManager* scene, Camera* camera, int x, int y);

#endif // RENDERER_H 