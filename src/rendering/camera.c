#include "rendering/camera.h"

// Initialize camera
void camera_init(Camera* camera, vec3 position, vec3 front, vec3 up) {
    // Set camera properties
    camera->position[0] = position[0];
    camera->position[1] = position[1];
    camera->position[2] = position[2];
    
    camera->worldUp[0] = up[0];
    camera->worldUp[1] = up[1];
    camera->worldUp[2] = up[2];
    
    camera->front[0] = front[0];
    camera->front[1] = front[1];
    camera->front[2] = front[2];
    
    // Set default values
    camera->yaw = -90.0f; // Looking along -Z axis
    camera->pitch = 0.0f;
    camera->fov = CAMERA_FOV;
    camera->nearPlane = CAMERA_NEAR;
    camera->farPlane = CAMERA_FAR;
    camera->aspectRatio = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
    
    // Calculate camera vectors
    camera_updateVectors(camera);
    
    // Initialize view and projection matrices
    camera_updateViewMatrix(camera);
    camera_updateProjection(camera, WINDOW_WIDTH, WINDOW_HEIGHT);
}

// Update camera vectors based on Euler angles
void camera_updateVectors(Camera* camera) {
    // Calculate front vector from Euler angles
    camera->front[0] = cos(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
    camera->front[1] = sin(glm_rad(camera->pitch));
    camera->front[2] = sin(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
    
    // Normalize front vector
    float length = sqrt(camera->front[0] * camera->front[0] + 
                       camera->front[1] * camera->front[1] + 
                       camera->front[2] * camera->front[2]);
    camera->front[0] /= length;
    camera->front[1] /= length;
    camera->front[2] /= length;
    
    // Calculate right vector
    camera->right[0] = camera->front[1] * camera->worldUp[2] - camera->front[2] * camera->worldUp[1];
    camera->right[1] = camera->front[2] * camera->worldUp[0] - camera->front[0] * camera->worldUp[2];
    camera->right[2] = camera->front[0] * camera->worldUp[1] - camera->front[1] * camera->worldUp[0];
    
    // Normalize right vector
    length = sqrt(camera->right[0] * camera->right[0] + 
                  camera->right[1] * camera->right[1] + 
                  camera->right[2] * camera->right[2]);
    camera->right[0] /= length;
    camera->right[1] /= length;
    camera->right[2] /= length;
    
    // Calculate up vector
    camera->up[0] = camera->right[1] * camera->front[2] - camera->right[2] * camera->front[1];
    camera->up[1] = camera->right[2] * camera->front[0] - camera->right[0] * camera->front[2];
    camera->up[2] = camera->right[0] * camera->front[1] - camera->right[1] * camera->front[0];
    
    // Normalize up vector
    length = sqrt(camera->up[0] * camera->up[0] + 
                  camera->up[1] * camera->up[1] + 
                  camera->up[2] * camera->up[2]);
    camera->up[0] /= length;
    camera->up[1] /= length;
    camera->up[2] /= length;
}

// Update view matrix
void camera_updateViewMatrix(Camera* camera) {
    // Calculate look-at target
    vec3 target = {
        camera->position[0] + camera->front[0],
        camera->position[1] + camera->front[1],
        camera->position[2] + camera->front[2]
    };
    
    // Calculate right and up vectors
    vec3 f = {
        target[0] - camera->position[0],
        target[1] - camera->position[1],
        target[2] - camera->position[2]
    };
    
    // Normalize f
    float lengthF = sqrt(f[0] * f[0] + f[1] * f[1] + f[2] * f[2]);
    f[0] /= lengthF;
    f[1] /= lengthF;
    f[2] /= lengthF;
    
    // Calculate s = f × up (cross product)
    vec3 s = {
        f[1] * camera->worldUp[2] - f[2] * camera->worldUp[1],
        f[2] * camera->worldUp[0] - f[0] * camera->worldUp[2],
        f[0] * camera->worldUp[1] - f[1] * camera->worldUp[0]
    };
    
    // Normalize s
    float lengthS = sqrt(s[0] * s[0] + s[1] * s[1] + s[2] * s[2]);
    s[0] /= lengthS;
    s[1] /= lengthS;
    s[2] /= lengthS;
    
    // Calculate u = s × f
    vec3 u = {
        s[1] * f[2] - s[2] * f[1],
        s[2] * f[0] - s[0] * f[2],
        s[0] * f[1] - s[1] * f[0]
    };
    
    // Build view matrix
    camera->viewMatrix[0] = s[0];
    camera->viewMatrix[4] = s[1];
    camera->viewMatrix[8] = s[2];
    camera->viewMatrix[12] = -dot(s, camera->position);
    
    camera->viewMatrix[1] = u[0];
    camera->viewMatrix[5] = u[1];
    camera->viewMatrix[9] = u[2];
    camera->viewMatrix[13] = -dot(u, camera->position);
    
    camera->viewMatrix[2] = -f[0];
    camera->viewMatrix[6] = -f[1];
    camera->viewMatrix[10] = -f[2];
    camera->viewMatrix[14] = dot(f, camera->position);
    
    camera->viewMatrix[3] = 0.0f;
    camera->viewMatrix[7] = 0.0f;
    camera->viewMatrix[11] = 0.0f;
    camera->viewMatrix[15] = 1.0f;
}

// Update projection matrix
void camera_updateProjection(Camera* camera, int width, int height) {
    camera->aspectRatio = (float)width / (float)height;
    
    // Calculate perspective projection matrix
    float tanHalfFovy = tan(glm_rad(camera->fov) / 2.0f);
    float f = 1.0f / tanHalfFovy;
    float nf = 1.0f / (camera->nearPlane - camera->farPlane);
    
    camera->projectionMatrix[0] = f / camera->aspectRatio;
    camera->projectionMatrix[1] = 0.0f;
    camera->projectionMatrix[2] = 0.0f;
    camera->projectionMatrix[3] = 0.0f;
    
    camera->projectionMatrix[4] = 0.0f;
    camera->projectionMatrix[5] = f;
    camera->projectionMatrix[6] = 0.0f;
    camera->projectionMatrix[7] = 0.0f;
    
    camera->projectionMatrix[8] = 0.0f;
    camera->projectionMatrix[9] = 0.0f;
    camera->projectionMatrix[10] = (camera->farPlane + camera->nearPlane) * nf;
    camera->projectionMatrix[11] = -1.0f;
    
    camera->projectionMatrix[12] = 0.0f;
    camera->projectionMatrix[13] = 0.0f;
    camera->projectionMatrix[14] = (2.0f * camera->farPlane * camera->nearPlane) * nf;
    camera->projectionMatrix[15] = 0.0f;
}

// Rotate camera
void camera_rotate(Camera* camera, float yawOffset, float pitchOffset) {
    camera->yaw += yawOffset;
    camera->pitch += pitchOffset;
    
    // Constrain pitch to avoid flipping
    if (camera->pitch > 89.0f) {
        camera->pitch = 89.0f;
    }
    if (camera->pitch < -89.0f) {
        camera->pitch = -89.0f;
    }
    
    // Update camera vectors and view matrix
    camera_updateVectors(camera);
    camera_updateViewMatrix(camera);
}

// Move camera
void camera_move(Camera* camera, vec3 direction, float speed) {
    // Calculate movement vector
    vec3 movement = {0.0f, 0.0f, 0.0f};
    
    if (direction[0] != 0) { // Strafe right/left
        movement[0] += camera->right[0] * direction[0] * speed;
        movement[1] += camera->right[1] * direction[0] * speed;
        movement[2] += camera->right[2] * direction[0] * speed;
    }
    
    if (direction[1] != 0) { // Move up/down
        movement[0] += camera->worldUp[0] * direction[1] * speed;
        movement[1] += camera->worldUp[1] * direction[1] * speed;
        movement[2] += camera->worldUp[2] * direction[1] * speed;
    }
    
    if (direction[2] != 0) { // Move forward/backward
        movement[0] += camera->front[0] * -direction[2] * speed;
        movement[1] += camera->front[1] * -direction[2] * speed;
        movement[2] += camera->front[2] * -direction[2] * speed;
    }
    
    // Apply movement
    camera->position[0] += movement[0];
    camera->position[1] += movement[1];
    camera->position[2] += movement[2];
    
    // Update view matrix
    camera_updateViewMatrix(camera);
}

// Get view matrix
void camera_getViewMatrix(Camera* camera, mat4 viewMatrix) {
    for (int i = 0; i < 16; i++) {
        viewMatrix[i] = camera->viewMatrix[i];
    }
}

// Get projection matrix
void camera_getProjectionMatrix(Camera* camera, mat4 projectionMatrix) {
    for (int i = 0; i < 16; i++) {
        projectionMatrix[i] = camera->projectionMatrix[i];
    }
}

// Get camera position
void camera_getPosition(Camera* camera, vec3 position) {
    position[0] = camera->position[0];
    position[1] = camera->position[1];
    position[2] = camera->position[2];
}

// Get camera front vector
void camera_getFront(Camera* camera, vec3 front) {
    front[0] = camera->front[0];
    front[1] = camera->front[1];
    front[2] = camera->front[2];
}

// Helper function: Calculate dot product
static float dot(vec3 a, vec3 b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
} 