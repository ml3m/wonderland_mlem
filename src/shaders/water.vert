#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;
out vec4 ClipSpace;
out vec3 ToCameraVector;
out vec2 DistortedTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 cameraPosition;

// Water properties
uniform float moveFactor;
uniform float waveStrength;
uniform float waveHeight;
uniform float waveSpeed;
uniform float waveFrequency;
uniform float time;

void main()
{
    vec3 position = aPos;
    
    // Apply wave displacement
    float waveX = sin(position.x * waveFrequency + time * waveSpeed) * waveHeight;
    float waveZ = cos(position.z * waveFrequency + time * waveSpeed) * waveHeight;
    position.y += waveX + waveZ;
    
    // Calculate model coordinates
    vec4 worldPos = model * vec4(position, 1.0);
    FragPos = worldPos.xyz;
    
    // Calculate normal based on wave derivatives
    float slopeX = waveHeight * waveFrequency * cos(position.x * waveFrequency + time * waveSpeed);
    float slopeZ = -waveHeight * waveFrequency * sin(position.z * waveFrequency + time * waveSpeed);
    vec3 waveNormal = normalize(vec3(-slopeX, 1.0, -slopeZ));
    
    // Transform normal to world space
    Normal = mat3(transpose(inverse(model))) * waveNormal;
    
    // Calculate position in clip space
    ClipSpace = projection * view * worldPos;
    gl_Position = ClipSpace;
    
    // Set texture coordinates
    TexCoords = aTexCoords;
    
    // Calculate distorted texture coordinates for DuDv map
    DistortedTexCoords = vec2(
        aTexCoords.x + moveFactor * 0.1,
        aTexCoords.y + moveFactor * 0.1
    );
    
    // Calculate vector from fragment to camera
    ToCameraVector = cameraPosition - FragPos;
} 