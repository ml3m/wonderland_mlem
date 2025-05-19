#include "rendering/renderer.h"
#include "scene/scene_manager.h"
#include "rendering/camera.h"

// Initialize renderer
void renderer_init(Renderer* renderer) {
    // Set default settings
    renderer->enableShadows = true;
    renderer->enableSSAO = true;
    renderer->enableFXAA = true;
    renderer->enableBloom = true;
    renderer->enableDoF = true;
    renderer->enableGodRays = true;
    renderer->showWireframe = false;
    
    renderer->shadowMapResolution = SHADOW_MAP_SIZE;
    renderer->shadowBias = 0.005f;
    
    renderer->exposureValue = 1.0f;
    renderer->bloomThreshold = 0.8f;
    renderer->bloomIntensity = 0.5f;
    renderer->dofFocalDistance = 20.0f;
    renderer->dofFocalRange = 10.0f;
    
    // Setup framebuffers
    renderer_setupFramebuffers(renderer);
    
    // Setup shaders
    renderer_setupShaders(renderer);
    
    // Setup quad
    renderer_setupQuad(renderer);
    
    // Setup SSAO
    renderer_setupSSAO(renderer);
}

// Clean up renderer resources
void renderer_cleanup(Renderer* renderer) {
    // Delete framebuffers
    glDeleteFramebuffers(1, &renderer->gBuffer);
    glDeleteFramebuffers(1, &renderer->ssaoBuffer);
    glDeleteFramebuffers(1, &renderer->ssaoBlurBuffer);
    glDeleteFramebuffers(1, &renderer->hdrBuffer);
    glDeleteFramebuffers(2, renderer->pingpongBuffers);
    
    // Delete textures
    glDeleteTextures(1, &renderer->gPosition);
    glDeleteTextures(1, &renderer->gNormal);
    glDeleteTextures(1, &renderer->gAlbedo);
    glDeleteTextures(1, &renderer->gMaterial);
    glDeleteTextures(1, &renderer->ssaoColorBuffer);
    glDeleteTextures(1, &renderer->ssaoBlurColorBuffer);
    glDeleteTextures(1, &renderer->hdrColorBuffer);
    glDeleteTextures(2, renderer->pingpongColorBuffers);
    glDeleteTextures(1, &renderer->ssaoNoiseTexture);
    
    // Delete renderbuffers
    glDeleteRenderbuffers(1, &renderer->depthRenderBuffer);
    
    // Delete VAO and VBO
    glDeleteVertexArrays(1, &renderer->quadVAO);
    glDeleteBuffers(1, &renderer->quadVBO);
    
    // Delete shaders
    glDeleteProgram(renderer->gBufferShader);
    glDeleteProgram(renderer->lightingShader);
    glDeleteProgram(renderer->ssaoShader);
    glDeleteProgram(renderer->ssaoBlurShader);
    glDeleteProgram(renderer->shadowMapShader);
    glDeleteProgram(renderer->skyboxShader);
    glDeleteProgram(renderer->terrainShader);
    glDeleteProgram(renderer->waterShader);
    glDeleteProgram(renderer->vegetationShader);
    glDeleteProgram(renderer->instancedShader);
    glDeleteProgram(renderer->particleShader);
    glDeleteProgram(renderer->postProcessShader);
    glDeleteProgram(renderer->blurShader);
    glDeleteProgram(renderer->compositShader);
    
    // Free SSAO kernel
    free(renderer->ssaoKernel);
}

// Setup framebuffers
void renderer_setupFramebuffers(Renderer* renderer) {
    // Create G-buffer
    glGenFramebuffers(1, &renderer->gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, renderer->gBuffer);
    
    // Position buffer
    glGenTextures(1, &renderer->gPosition);
    glBindTexture(GL_TEXTURE_2D, renderer->gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderer->gPosition, 0);
    
    // Normal buffer
    glGenTextures(1, &renderer->gNormal);
    glBindTexture(GL_TEXTURE_2D, renderer->gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, renderer->gNormal, 0);
    
    // Albedo buffer
    glGenTextures(1, &renderer->gAlbedo);
    glBindTexture(GL_TEXTURE_2D, renderer->gAlbedo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, renderer->gAlbedo, 0);
    
    // Material properties buffer (Roughness, Metallic, AO)
    glGenTextures(1, &renderer->gMaterial);
    glBindTexture(GL_TEXTURE_2D, renderer->gMaterial);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, renderer->gMaterial, 0);
    
    // Tell OpenGL which color attachments we'll use
    unsigned int attachments[4] = { 
        GL_COLOR_ATTACHMENT0, 
        GL_COLOR_ATTACHMENT1, 
        GL_COLOR_ATTACHMENT2, 
        GL_COLOR_ATTACHMENT3 
    };
    glDrawBuffers(4, attachments);
    
    // Depth renderbuffer
    glGenRenderbuffers(1, &renderer->depthRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderer->depthRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WINDOW_WIDTH, WINDOW_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderer->depthRenderBuffer);
    
    // Check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        fprintf(stderr, "G-Buffer framebuffer is not complete!\n");
    }
    
    // Create SSAO framebuffer
    glGenFramebuffers(1, &renderer->ssaoBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, renderer->ssaoBuffer);
    
    // SSAO color buffer
    glGenTextures(1, &renderer->ssaoColorBuffer);
    glBindTexture(GL_TEXTURE_2D, renderer->ssaoColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RED, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderer->ssaoColorBuffer, 0);
    
    // Check SSAO framebuffer
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        fprintf(stderr, "SSAO framebuffer is not complete!\n");
    }
    
    // Create SSAO blur framebuffer
    glGenFramebuffers(1, &renderer->ssaoBlurBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, renderer->ssaoBlurBuffer);
    
    // SSAO blur color buffer
    glGenTextures(1, &renderer->ssaoBlurColorBuffer);
    glBindTexture(GL_TEXTURE_2D, renderer->ssaoBlurColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RED, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderer->ssaoBlurColorBuffer, 0);
    
    // Check SSAO blur framebuffer
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        fprintf(stderr, "SSAO blur framebuffer is not complete!\n");
    }
    
    // Create HDR framebuffer
    glGenFramebuffers(1, &renderer->hdrBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, renderer->hdrBuffer);
    
    // HDR color buffer
    glGenTextures(1, &renderer->hdrColorBuffer);
    glBindTexture(GL_TEXTURE_2D, renderer->hdrColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderer->hdrColorBuffer, 0);
    
    // Reuse depth buffer
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderer->depthRenderBuffer);
    
    // Check HDR framebuffer
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        fprintf(stderr, "HDR framebuffer is not complete!\n");
    }
    
    // Create ping-pong framebuffers for gaussian blur
    glGenFramebuffers(2, renderer->pingpongBuffers);
    glGenTextures(2, renderer->pingpongColorBuffers);
    
    for (int i = 0; i < 2; i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, renderer->pingpongBuffers[i]);
        glBindTexture(GL_TEXTURE_2D, renderer->pingpongColorBuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderer->pingpongColorBuffers[i], 0);
        
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            fprintf(stderr, "Ping-pong framebuffer %d is not complete!\n", i);
        }
    }
    
    // Unbind framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// Setup shaders
void renderer_setupShaders(Renderer* renderer) {
    // Load shaders
    renderer->gBufferShader = shader_load("src/shaders/gbuffer.vert", "src/shaders/gbuffer.frag");
    renderer->lightingShader = shader_load("src/shaders/lighting.vert", "src/shaders/lighting.frag");
    renderer->ssaoShader = shader_load("src/shaders/ssao.vert", "src/shaders/ssao.frag");
    renderer->ssaoBlurShader = shader_load("src/shaders/ssao_blur.vert", "src/shaders/ssao_blur.frag");
    renderer->shadowMapShader = shader_load("src/shaders/shadow_map.vert", "src/shaders/shadow_map.frag");
    renderer->skyboxShader = shader_load("src/shaders/skybox.vert", "src/shaders/skybox.frag");
    renderer->terrainShader = shader_load("src/shaders/terrain.vert", "src/shaders/terrain.frag");
    renderer->waterShader = shader_load("src/shaders/water.vert", "src/shaders/water.frag");
    renderer->vegetationShader = shader_load("src/shaders/vegetation.vert", "src/shaders/vegetation.frag");
    renderer->instancedShader = shader_load("src/shaders/instanced.vert", "src/shaders/instanced.frag");
    renderer->particleShader = shader_load("src/shaders/particle.vert", "src/shaders/particle.frag");
    renderer->postProcessShader = shader_load("src/shaders/post_process.vert", "src/shaders/post_process.frag");
    renderer->blurShader = shader_load("src/shaders/blur.vert", "src/shaders/blur.frag");
    renderer->compositShader = shader_load("src/shaders/composit.vert", "src/shaders/composit.frag");
}

// Setup screen-space quad
void renderer_setupQuad(Renderer* renderer) {
    float quadVertices[] = {
        // positions        // texture Coords
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };
    
    // Setup screen quad VAO
    glGenVertexArrays(1, &renderer->quadVAO);
    glGenBuffers(1, &renderer->quadVBO);
    glBindVertexArray(renderer->quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}

// Setup SSAO
void renderer_setupSSAO(Renderer* renderer) {
    // Generate SSAO kernel
    renderer->ssaoKernelSize = 64;
    renderer->ssaoKernel = malloc(sizeof(vec3) * renderer->ssaoKernelSize);
    
    for (unsigned int i = 0; i < renderer->ssaoKernelSize; i++) {
        vec3 sample = {
            (float)rand() / RAND_MAX * 2.0f - 1.0f,
            (float)rand() / RAND_MAX * 2.0f - 1.0f,
            (float)rand() / RAND_MAX
        };
        
        // Normalize
        float scale = (float)i / renderer->ssaoKernelSize;
        scale = 0.1f + scale * scale * 0.9f; // Lerp
        
        sample[0] *= scale;
        sample[1] *= scale;
        sample[2] *= scale;
        
        renderer->ssaoKernel[i][0] = sample[0];
        renderer->ssaoKernel[i][1] = sample[1];
        renderer->ssaoKernel[i][2] = sample[2];
    }
    
    // Generate noise texture
    vec3 ssaoNoise[16];
    for (unsigned int i = 0; i < 16; i++) {
        vec3 noise = {
            (float)rand() / RAND_MAX * 2.0f - 1.0f,
            (float)rand() / RAND_MAX * 2.0f - 1.0f,
            0.0f
        };
        
        ssaoNoise[i][0] = noise[0];
        ssaoNoise[i][1] = noise[1];
        ssaoNoise[i][2] = noise[2];
    }
    
    glGenTextures(1, &renderer->ssaoNoiseTexture);
    glBindTexture(GL_TEXTURE_2D, renderer->ssaoNoiseTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

// Main render function
void renderer_render(Renderer* renderer, SceneManager* scene, Camera* camera, float timeOfDay, WeatherType weather) {
    // 1. Render shadow maps
    if (renderer->enableShadows) {
        renderer_renderShadowMaps(renderer, scene);
    }
    
    // 2. Geometry pass (fill G-buffer)
    renderer_geometryPass(renderer, scene, camera);
    
    // 3. SSAO pass
    if (renderer->enableSSAO) {
        renderer_ssaoPass(renderer, camera);
    }
    
    // 4. Lighting pass
    renderer_lightingPass(renderer, scene, camera, timeOfDay);
    
    // 5. Transparency pass (water, particles)
    renderer_transparencyPass(renderer, scene, camera, timeOfDay);
    
    // 6. Post-process pass
    renderer_postProcessPass(renderer);
}

// Geometry pass
void renderer_geometryPass(Renderer* renderer, SceneManager* scene, Camera* camera) {
    // Bind G-buffer
    glBindFramebuffer(GL_FRAMEBUFFER, renderer->gBuffer);
    
    // Clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Set viewport
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    
    // Wireframe mode
    if (renderer->showWireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    
    // Render the scene
    renderer_renderScene(renderer, scene, camera, false);
    
    // Reset polygon mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

// Simple scene renderer
void renderer_renderScene(Renderer* renderer, SceneManager* scene, Camera* camera, bool depthOnly) {
    // Get view matrix
    mat4 viewMatrix;
    camera_getViewMatrix(camera, viewMatrix);
    
    // Get projection matrix
    mat4 projectionMatrix;
    camera_getProjectionMatrix(camera, projectionMatrix);
    
    // Use appropriate shader
    GLuint shader = depthOnly ? renderer->shadowMapShader : renderer->gBufferShader;
    shader_use(shader);
    
    // Set matrices
    if (!depthOnly) {
        shader_setMat4(shader, "view", viewMatrix);
        shader_setMat4(shader, "projection", projectionMatrix);
    }
    
    // Render terrain
    renderer_renderTerrain(renderer, &scene->terrain, camera, depthOnly);
    
    // Render objects
    for (size_t i = 0; i < scene->cottageCount; i++) {
        renderer_renderObject(renderer, &scene->cottages[i], camera, depthOnly);
    }
    
    for (size_t i = 0; i < scene->ruinCount; i++) {
        renderer_renderObject(renderer, &scene->ruins[i], camera, depthOnly);
    }
    
    for (size_t i = 0; i < scene->bridgeCount; i++) {
        renderer_renderObject(renderer, &scene->bridges[i], camera, depthOnly);
    }
    
    // Render instanced vegetation
    renderer_renderInstancedObjects(renderer, scene->trees, scene->treeCount, camera, depthOnly);
    renderer_renderInstancedObjects(renderer, scene->flowers, scene->flowerCount, camera, depthOnly);
    renderer_renderInstancedObjects(renderer, scene->mushrooms, scene->mushroomCount, camera, depthOnly);
    renderer_renderInstancedObjects(renderer, scene->lanterns, scene->lanternCount, camera, depthOnly);
    
    // Render skybox (only in non-depth pass)
    if (!depthOnly) {
        renderer_renderSkybox(renderer, &scene->skybox, camera);
    }
}

// Pick object under mouse cursor
void renderer_pick(Renderer* renderer, SceneManager* scene, Camera* camera, int x, int y) {
    // Not implemented yet
} 