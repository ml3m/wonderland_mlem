#version 410 core

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in vec4 ClipSpace;
in vec3 ToCameraVector;
in vec2 DistortedTexCoords;

out vec4 FragColor;

// Water textures
uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D refractionDepthTexture;
uniform sampler2D dudvMap;
uniform sampler2D normalMap;
uniform sampler2D flowMap;

// Water properties
uniform float moveFactor;
uniform float waveStrength;
uniform float waterDepth;
uniform vec3 waterColor;
uniform float waterAlpha;
uniform float shineDamper;
uniform float reflectivity;

// Lighting
uniform vec3 lightDirection;
uniform vec3 lightColor;

// Camera
uniform vec3 cameraPosition;
uniform float nearPlane;
uniform float farPlane;

// Convert clip space coordinates to texture coordinates
vec2 clipToTexCoords(vec4 clipSpace) {
    vec2 ndc = (clipSpace.xy / clipSpace.w) * 0.5 + 0.5;
    return ndc;
}

// Convert depth buffer value to linear depth
float linearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0; // Convert to NDC
    return (2.0 * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane));
}

void main()
{
    // Normalize vectors
    vec3 normal = normalize(Normal);
    vec3 viewVector = normalize(ToCameraVector);
    
    // Calculate distortion from dudv map
    vec2 distortedTexCoords = texture(dudvMap, vec2(DistortedTexCoords.x + moveFactor, DistortedTexCoords.y)).rg * 0.1;
    distortedTexCoords = TexCoords + vec2(distortedTexCoords.x, distortedTexCoords.y + moveFactor);
    vec2 totalDistortion = (texture(dudvMap, distortedTexCoords).rg * 2.0 - 1.0) * waveStrength;
    
    // Calculate reflection and refraction coordinates
    vec2 reflectionTexCoords = clipToTexCoords(ClipSpace);
    vec2 refractionTexCoords = clipToTexCoords(ClipSpace);
    
    // Apply distortion to texture coordinates
    reflectionTexCoords += totalDistortion;
    refractionTexCoords += totalDistortion;
    
    // Clamp to prevent sampling outside of texture
    reflectionTexCoords = clamp(reflectionTexCoords, 0.001, 0.999);
    refractionTexCoords = clamp(refractionTexCoords, 0.001, 0.999);
    
    // Sample normal map
    vec4 normalMapColor = texture(normalMap, distortedTexCoords);
    vec3 waterNormal = vec3(normalMapColor.r * 2.0 - 1.0, normalMapColor.b * 1.0, normalMapColor.g * 2.0 - 1.0);
    waterNormal = normalize(waterNormal);
    
    // Calculate Fresnel effect
    float refractiveFactor = dot(viewVector, waterNormal);
    refractiveFactor = pow(refractiveFactor, 1.0);
    refractiveFactor = clamp(refractiveFactor, 0.0, 1.0);
    
    // Sample reflection and refraction
    vec4 reflectionColor = texture(reflectionTexture, reflectionTexCoords);
    vec4 refractionColor = texture(refractionTexture, refractionTexCoords);
    
    // Get water depth from refraction depth texture
    float depth = texture(refractionDepthTexture, refractionTexCoords).r;
    float floorDistance = linearizeDepth(depth);
    float waterSurfaceDistance = linearizeDepth(gl_FragCoord.z);
    float waterDepthFactor = floorDistance - waterSurfaceDistance;
    
    // Apply water color based on depth
    vec4 finalRefraction = mix(refractionColor, vec4(waterColor, 1.0), clamp(waterDepthFactor / waterDepth, 0.0, 1.0));
    
    // Calculate specular highlights
    vec3 halfwayDir = normalize(-lightDirection + viewVector);
    float specular = pow(max(dot(waterNormal, halfwayDir), 0.0), shineDamper);
    vec3 specularHighlights = lightColor * specular * reflectivity;
    
    // Mix reflection and refraction
    FragColor = mix(reflectionColor, finalRefraction, refractiveFactor);
    
    // Add specular highlights
    FragColor = FragColor + vec4(specularHighlights, 0.0);
    
    // Set alpha
    FragColor.a = waterAlpha;
} 