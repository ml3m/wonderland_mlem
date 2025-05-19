#ifndef CONFIG_H
#define CONFIG_H

// Window configuration
#define WINDOW_TITLE "Enchanted Wonderlands"
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

// Terrain configuration
#define TERRAIN_SIZE 1024
#define TERRAIN_SCALE 100.0f
#define TERRAIN_HEIGHT_SCALE 20.0f
#define TERRAIN_MAX_LOD 4

// Water configuration
#define WATER_GRID_SIZE 256
#define WATER_TILE_SIZE 0.5f
#define WATER_HEIGHT 5.0f
#define WATER_WAVE_SPEED 0.05f

// Vegetation configuration
#define TREE_INSTANCES 1000
#define GRASS_INSTANCES 50000
#define FLOWER_INSTANCES 2000
#define MUSHROOM_INSTANCES 500

// Lighting configuration
#define MAX_LIGHTS 64
#define SHADOW_MAP_SIZE 4096

// Camera configuration
#define CAMERA_FOV 60.0f
#define CAMERA_NEAR 0.1f
#define CAMERA_FAR 2000.0f
#define CAMERA_MOVE_SPEED 10.0f
#define CAMERA_MOUSE_SENSITIVITY 0.1f

// Particle system configuration
#define MAX_PARTICLES 100000
#define PARTICLE_EMIT_RATE 100

// Debug configuration
#define DEBUG_MODE 1
#define WIREFRAME_MODE 0

// Weather and time configuration
#define DAY_LENGTH 600.0f  // 10 minutes per day
#define RAIN_PROBABILITY 0.3f
#define FOG_PROBABILITY 0.2f

#endif // CONFIG_H 