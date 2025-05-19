#ifndef TERRAIN_H
#define TERRAIN_H

#include "wonderlands.h"

// Terrain patch (for LOD)
typedef struct {
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    int indexCount;
    vec3 center;
    float radius;
    int lod;
} TerrainPatch;

// Biome types
typedef enum {
    BIOME_MEADOW,
    BIOME_FOREST,
    BIOME_ROCKY,
    BIOME_COUNT
} BiomeType;

// Biome definition
typedef struct {
    float minHeight;
    float maxHeight;
    float minSlope;
    float maxSlope;
    vec3 baseColor;
    GLuint diffuseMap;
    GLuint normalMap;
    GLuint roughnessMap;
    float tiling;
} Biome;

// Terrain structure
typedef struct {
    // Mesh data
    TerrainPatch* patches;
    int patchCount;
    int patchesPerSide;
    
    // Heightmap data
    GLuint heightMap;
    float* heightData;
    int heightMapSize;
    float heightScale;
    
    // Noise parameters
    int octaves;
    float persistence;
    float lacunarity;
    float scale;
    int seed;
    
    // Material maps
    GLuint normalMap;
    GLuint splatMap;
    
    // Biome settings
    Biome biomes[BIOME_COUNT];
    
    // Terrain size and scale
    float size;
    float scale;
    float maxHeight;
    
    // Lighting settings
    GLuint aoMap;
    
    // LOD settings
    int maxLOD;
    float lodDistanceFactor;
} Terrain;

// Function prototypes
void terrain_init(Terrain* terrain, int size, float scale, float heightScale, int maxLOD);
void terrain_cleanup(Terrain* terrain);
void terrain_generate(Terrain* terrain);
void terrain_setupPatches(Terrain* terrain);
void terrain_updateLOD(Terrain* terrain, vec3 cameraPosition);
float terrain_getHeight(Terrain* terrain, float x, float z);
float terrain_getSlope(Terrain* terrain, float x, float z);
BiomeType terrain_getBiomeAt(Terrain* terrain, float x, float z);
void terrain_calculateNormalMap(Terrain* terrain);
void terrain_generateSplatMap(Terrain* terrain);
void terrain_setupBiomes(Terrain* terrain);
void terrain_renderPatch(Terrain* terrain, TerrainPatch* patch, GLuint shader);

#endif // TERRAIN_H 