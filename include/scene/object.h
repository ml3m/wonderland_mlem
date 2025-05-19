#ifndef OBJECT_H
#define OBJECT_H

#include "wonderlands.h"

// Forward declarations
typedef struct Mesh Mesh;
typedef struct Material Material;

// Light types
typedef enum {
    LIGHT_DIRECTIONAL,
    LIGHT_POINT,
    LIGHT_SPOT
} LightType;

// Light structure
typedef struct {
    LightType type;
    vec3 position;
    vec3 direction;
    vec3 color;
    float intensity;
    float range;
    float innerCutoff;
    float outerCutoff;
    bool castShadows;
    mat4 lightSpaceMatrix;
    GLuint shadowMap;
} Light;

// Object transformation
typedef struct {
    vec3 position;
    vec3 rotation;
    vec3 scale;
    mat4 modelMatrix;
} Transform;

// Object structure
typedef struct {
    char* name;
    Transform transform;
    Mesh* mesh;
    Material* material;
    bool isVisible;
    bool isStatic;
    
    // Instance data
    bool isInstanced;
    size_t instanceCount;
    Transform* instances;
    GLuint instanceBuffer;
    
    // Animation data
    bool isAnimated;
    float animationTime;
    float windFactor;
    
    // Physics properties
    bool hasPhysics;
    float mass;
    float restitution;
    
    // For tree/vegetation sway
    float swayAmount;
    float swaySpeed;
} Object;

// Function prototypes
void object_init(Object* object, const char* name, Mesh* mesh, Material* material);
void object_update(Object* object, float deltaTime, float windStrength, float windDirection);
void object_updateTransform(Object* object);
void object_addInstance(Object* object, vec3 position, vec3 rotation, vec3 scale);
void object_setupInstances(Object* object);
void object_cleanup(Object* object);

// Light functions
void light_init(Light* light, LightType type, vec3 position, vec3 color, float intensity);
void light_update(Light* light, vec3 position, float intensity);
void light_setupShadowMap(Light* light, int resolution);
void light_calculateLightSpaceMatrix(Light* light, vec3 sceneCenter, float sceneRadius);

#endif // OBJECT_H 