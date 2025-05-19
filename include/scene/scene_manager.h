#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "wonderlands.h"
#include "scene/object.h"
#include "rendering/terrain.h"
#include "rendering/water.h"
#include "rendering/skybox.h"
#include "rendering/particles.h"

// Weather types
typedef enum {
    WEATHER_CLEAR,
    WEATHER_RAIN,
    WEATHER_FOG
} WeatherType;

// Structure to manage the scene
typedef struct {
    // Scene components
    Terrain terrain;
    Water water;
    Skybox skybox;
    ParticleSystem particles;
    
    // Object collections
    Object* trees;
    size_t treeCount;
    
    Object* flowers;
    size_t flowerCount;
    
    Object* mushrooms;
    size_t mushroomCount;
    
    Object* cottages;
    size_t cottageCount;
    
    Object* ruins;
    size_t ruinCount;
    
    Object* bridges;
    size_t bridgeCount;
    
    Object* lanterns;
    size_t lanternCount;
    
    // Lights
    Light* lights;
    size_t lightCount;
    
    // Current weather
    WeatherType weather;
    float weatherIntensity;
    
    // Physics settings
    float windStrength;
    float windDirection;
} SceneManager;

// Function prototypes
void sceneManager_init(SceneManager* scene);
void sceneManager_update(SceneManager* scene, float deltaTime, float timeOfDay, WeatherType weather);
void sceneManager_cleanup(SceneManager* scene);
void sceneManager_generateTerrain(SceneManager* scene);
void sceneManager_createWater(SceneManager* scene);
void sceneManager_populateVegetation(SceneManager* scene);
void sceneManager_placeStructures(SceneManager* scene);
void sceneManager_setupLights(SceneManager* scene);
void sceneManager_updateWeather(SceneManager* scene, float deltaTime, WeatherType weather);
void sceneManager_updateLights(SceneManager* scene, float timeOfDay);
void sceneManager_updateWind(SceneManager* scene, float deltaTime);

#endif // SCENE_MANAGER_H 