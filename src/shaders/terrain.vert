#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;
out mat3 TBN;
out float Height;
out float Slope;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float terrainHeight;

void main()
{
    // Pass height and slope data to fragment shader
    Height = aPos.y / terrainHeight;
    
    // Calculate slope from normal (dot with up vector)
    Slope = 1.0 - dot(normalize(aNormal), vec3(0.0, 1.0, 0.0));
    
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;
    
    // Calculate TBN matrix for normal mapping
    vec3 T = normalize(mat3(model) * aTangent);
    vec3 N = normalize(mat3(model) * aNormal);
    vec3 B = cross(N, T);
    TBN = mat3(T, B, N);
    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
} 