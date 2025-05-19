#version 410 core

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedo;
layout (location = 3) out vec4 gMaterial; // R: roughness, G: metallic, B: AO

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in mat3 TBN;

// Material properties
uniform sampler2D texture_diffuse;
uniform sampler2D texture_normal;
uniform sampler2D texture_roughness;
uniform sampler2D texture_metallic;
uniform sampler2D texture_ao;

// Material flags
uniform bool hasNormalMap;
uniform bool hasRoughnessMap;
uniform bool hasMetallicMap;
uniform bool hasAOMap;

// Default material values
uniform float roughness = 0.5;
uniform float metallic = 0.0;
uniform float ao = 1.0;

void main()
{
    // Store position in world space
    gPosition = vec4(FragPos, 1.0);
    
    // Get normal from normal map if available
    vec3 norm;
    if (hasNormalMap) {
        norm = texture(texture_normal, TexCoords).rgb;
        norm = normalize(norm * 2.0 - 1.0);
        norm = normalize(TBN * norm);
    } else {
        norm = normalize(Normal);
    }
    gNormal = vec4(norm, 1.0);
    
    // Store diffuse albedo
    gAlbedo = texture(texture_diffuse, TexCoords);
    
    // Store material properties
    float roughnessValue = hasRoughnessMap ? texture(texture_roughness, TexCoords).r : roughness;
    float metallicValue = hasMetallicMap ? texture(texture_metallic, TexCoords).r : metallic;
    float aoValue = hasAOMap ? texture(texture_ao, TexCoords).r : ao;
    
    gMaterial = vec4(roughnessValue, metallicValue, aoValue, 1.0);
} 