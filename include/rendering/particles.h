#ifndef PARTICLES_H
#define PARTICLES_H

#include "wonderlands.h"

// Particle types
typedef enum {
    PARTICLE_DUST,
    PARTICLE_LEAF,
    PARTICLE_FIREFLY,
    PARTICLE_RAIN,
    PARTICLE_SPLASH,
    PARTICLE_MIST,
    PARTICLE_SNOW
} ParticleType;

// Particle emitter shapes
typedef enum {
    EMITTER_POINT,
    EMITTER_SPHERE,
    EMITTER_BOX,
    EMITTER_CIRCLE,
    EMITTER_PLANE
} EmitterShape;

// Individual particle data
typedef struct {
    vec3 position;
    vec3 velocity;
    vec4 color;
    float size;
    float rotation;
    float rotationSpeed;
    float lifetime;
    float maxLifetime;
    float distanceFromCamera;
    bool active;
} Particle;

// Emitter settings
typedef struct {
    ParticleType type;
    EmitterShape shape;
    vec3 position;
    vec3 dimensions;
    float radius;
    float emitRate;
    float emitAccumulator;
    vec3 minVelocity;
    vec3 maxVelocity;
    vec3 acceleration;
    vec4 startColor;
    vec4 endColor;
    float minSize;
    float maxSize;
    float minLifetime;
    float maxLifetime;
    float minRotationSpeed;
    float maxRotationSpeed;
    bool randomizeRotation;
    bool active;
    bool looping;
    float duration;
    float time;
} ParticleEmitter;

// Particle system
typedef struct {
    // Particle data
    Particle* particles;
    unsigned int particleCount;
    unsigned int maxParticles;
    
    // Emitters
    ParticleEmitter* emitters;
    unsigned int emitterCount;
    
    // OpenGL data
    GLuint vao;
    GLuint vbo;
    GLuint texture;
    
    // Instance data (for GPU-based particles)
    GLuint instanceVBO;
    GLfloat* instanceData;
    
    // Sorting
    unsigned int* indices;
    
    // Physics
    vec3 wind;
    bool usePhysics;
    bool collideWithTerrain;
    
    // Rendering
    bool additiveBlending;
    bool sortParticles;
    
    // Particle counts by type
    unsigned int dustParticleCount;
    unsigned int leafParticleCount;
    unsigned int fireflyParticleCount;
    unsigned int rainParticleCount;
    unsigned int splashParticleCount;
    unsigned int mistParticleCount;
    unsigned int snowParticleCount;
    
    // Textures for different particle types
    GLuint dustTexture;
    GLuint leafTexture;
    GLuint fireflyTexture;
    GLuint rainTexture;
    GLuint splashTexture;
    GLuint mistTexture;
    GLuint snowTexture;
} ParticleSystem;

// Function prototypes
void particleSystem_init(ParticleSystem* system, unsigned int maxParticles);
void particleSystem_cleanup(ParticleSystem* system);
void particleSystem_setupBuffers(ParticleSystem* system);
void particleSystem_setupTextures(ParticleSystem* system);
void particleSystem_update(ParticleSystem* system, float deltaTime, vec3 cameraPosition, float timeOfDay, WeatherType weather);
void particleSystem_render(ParticleSystem* system, Camera* camera, GLuint shader);
int particleSystem_addEmitter(ParticleSystem* system, ParticleType type, EmitterShape shape, vec3 position);
void particleSystem_removeEmitter(ParticleSystem* system, int index);
void particleSystem_updateEmitter(ParticleSystem* system, int index, float deltaTime);
void particleSystem_emit(ParticleSystem* system, ParticleEmitter* emitter, unsigned int count);
void particleSystem_updateParticles(ParticleSystem* system, float deltaTime, vec3 cameraPosition);
void particleSystem_sortParticles(ParticleSystem* system, vec3 cameraPosition);
void particleSystem_updateBuffers(ParticleSystem* system);
void particleSystem_setWind(ParticleSystem* system, vec3 wind);
void particleSystem_setCollision(ParticleSystem* system, bool collideWithTerrain);
void particleSystem_setupWeatherParticles(ParticleSystem* system, WeatherType weather);

#endif // PARTICLES_H 