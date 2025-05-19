#ifndef FLUID_SIMULATION_H
#define FLUID_SIMULATION_H

#include "wonderlands.h"

// Fluid simulation grid cell
typedef struct {
    float density;
    float velocityX;
    float velocityY;
    float velocityPrev[2];
    float pressure;
    float divergence;
    float obstacle;
} FluidCell;

// Fluid simulation settings
typedef struct {
    int gridSize;
    float cellSize;
    float diffusion;
    float viscosity;
    float timeStep;
    int iterations;
    bool useVorticity;
    float vorticityStrength;
    
    // Boundary conditions
    bool periodicBoundary;
    
    // External forces
    vec2* forcePositions;
    vec2* forceDirections;
    float* forceMagnitudes;
    int forceCount;
    
    // Shader programs
    GLuint advectionShader;
    GLuint diffusionShader;
    GLuint pressureShader;
    GLuint gradientShader;
    GLuint divergenceShader;
    GLuint vorticityShader;
    GLuint forceShader;
    GLuint boundaryShader;
    
    // Fluid textures
    GLuint velocityTexture;
    GLuint prevVelocityTexture;
    GLuint pressureTexture;
    GLuint divergenceTexture;
    GLuint obstacleTexture;
    GLuint vorticityTexture;
    
    // Framebuffers
    GLuint velocityFBO;
    GLuint pressureFBO;
    GLuint divergenceFBO;
    GLuint vorticityFBO;
    
    // Quad for rendering
    GLuint quadVAO;
    GLuint quadVBO;
} FluidSimulation;

// Function prototypes
void fluidSim_init(FluidSimulation* fluid, int gridSize, float cellSize);
void fluidSim_cleanup(FluidSimulation* fluid);
void fluidSim_setupShaders(FluidSimulation* fluid);
void fluidSim_setupTextures(FluidSimulation* fluid);
void fluidSim_setupFramebuffers(FluidSimulation* fluid);
void fluidSim_setupQuad(FluidSimulation* fluid);
void fluidSim_setObstacles(FluidSimulation* fluid, float* obstacleData);
void fluidSim_update(FluidSimulation* fluid, float deltaTime);
void fluidSim_advect(FluidSimulation* fluid);
void fluidSim_diffuse(FluidSimulation* fluid);
void fluidSim_computeDivergence(FluidSimulation* fluid);
void fluidSim_computePressure(FluidSimulation* fluid);
void fluidSim_applyPressureGradient(FluidSimulation* fluid);
void fluidSim_applyVorticity(FluidSimulation* fluid);
void fluidSim_applyExternalForces(FluidSimulation* fluid);
void fluidSim_applyBoundaryConditions(FluidSimulation* fluid);
void fluidSim_addForce(FluidSimulation* fluid, float x, float y, float dirX, float dirY, float magnitude);
void fluidSim_getVelocity(FluidSimulation* fluid, float x, float y, float* velocityX, float* velocityY);

#endif // FLUID_SIMULATION_H 