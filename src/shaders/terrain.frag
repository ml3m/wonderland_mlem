#version 410 core

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in mat3 TBN;
in float Height;
in float Slope;

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedo;
layout (location = 3) out vec4 gMaterial;

// Biome textures
uniform sampler2D meadowDiffuse;
uniform sampler2D meadowNormal;
uniform sampler2D meadowRoughness;

uniform sampler2D forestDiffuse;
uniform sampler2D forestNormal;
uniform sampler2D forestRoughness;

uniform sampler2D rockyDiffuse;
uniform sampler2D rockyNormal;
uniform sampler2D rockyRoughness;

// Utility textures
uniform sampler2D splatMap;
uniform sampler2D aoMap;

// Scale factors
uniform float meadowScale = 20.0;
uniform float forestScale = 25.0;
uniform float rockyScale = 15.0;

// Biome parameters (can also be computed in shader)
uniform float meadowMaxHeight = 0.3;
uniform float meadowMaxSlope = 0.2;
uniform float forestMaxHeight = 0.7;
uniform float forestMaxSlope = 0.5;

void main()
{
    // Store position
    gPosition = vec4(FragPos, 1.0);
    
    // Calculate biome weights based on height and slope
    float meadowWeight = (1.0 - smoothstep(0.0, meadowMaxHeight, Height)) * (1.0 - smoothstep(0.0, meadowMaxSlope, Slope));
    float forestWeight = smoothstep(0.1, 0.3, Height) * (1.0 - smoothstep(0.3, forestMaxHeight, Height)) * (1.0 - smoothstep(0.15, forestMaxSlope, Slope));
    float rockyWeight = smoothstep(0.5, 0.7, Height) + smoothstep(0.4, 0.6, Slope);
    
    // Normalize weights
    float totalWeight = meadowWeight + forestWeight + rockyWeight;
    if (totalWeight > 0.0) {
        meadowWeight /= totalWeight;
        forestWeight /= totalWeight;
        rockyWeight /= totalWeight;
    } else {
        // Default to rocky if no biome matches
        rockyWeight = 1.0;
    }
    
    // Sample textures with appropriate scales
    vec2 meadowUV = TexCoords * meadowScale;
    vec2 forestUV = TexCoords * forestScale;
    vec2 rockyUV = TexCoords * rockyScale;
    
    // Sample diffuse textures
    vec4 meadowColor = texture(meadowDiffuse, meadowUV);
    vec4 forestColor = texture(forestDiffuse, forestUV);
    vec4 rockyColor = texture(rockyDiffuse, rockyUV);
    
    // Blend diffuse colors
    vec4 diffuseColor = meadowColor * meadowWeight + forestColor * forestWeight + rockyColor * rockyWeight;
    
    // Sample normal maps
    vec3 meadowNormalMap = texture(meadowNormal, meadowUV).rgb;
    vec3 forestNormalMap = texture(forestNormal, forestUV).rgb;
    vec3 rockyNormalMap = texture(rockyNormal, rockyUV).rgb;
    
    // Convert normal maps from [0,1] to [-1,1] range
    meadowNormalMap = normalize(meadowNormalMap * 2.0 - 1.0);
    forestNormalMap = normalize(forestNormalMap * 2.0 - 1.0);
    rockyNormalMap = normalize(rockyNormalMap * 2.0 - 1.0);
    
    // Blend normal maps (this is simplified and not physically correct)
    vec3 normalMap = meadowNormalMap * meadowWeight + forestNormalMap * forestWeight + rockyNormalMap * rockyWeight;
    normalMap = normalize(normalMap);
    
    // Transform normal map to world space using TBN matrix
    vec3 worldNormal = normalize(TBN * normalMap);
    
    // Sample roughness maps
    float meadowRoughnessValue = texture(meadowRoughness, meadowUV).r;
    float forestRoughnessValue = texture(forestRoughness, forestUV).r;
    float rockyRoughnessValue = texture(rockyRoughness, rockyUV).r;
    
    // Blend roughness values
    float roughnessValue = meadowRoughnessValue * meadowWeight + forestRoughnessValue * forestWeight + rockyRoughnessValue * rockyWeight;
    
    // Get ambient occlusion
    float aoValue = texture(aoMap, TexCoords).r;
    
    // Output to G-buffer
    gNormal = vec4(worldNormal, 1.0);
    gAlbedo = diffuseColor;
    gMaterial = vec4(roughnessValue, 0.0, aoValue, 1.0); // Terrain is not metallic (0.0)
} 