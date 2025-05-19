#ifndef CAMERA_H
#define CAMERA_H

#include "wonderlands.h"

typedef float vec3[3];
typedef float vec4[4];
typedef float mat4[16];

typedef struct {
    vec3 position;
    vec3 front;
    vec3 up;
    vec3 right;
    vec3 worldUp;
    float yaw;
    float pitch;
    float fov;
    float aspectRatio;
    float nearPlane;
    float farPlane;
    mat4 viewMatrix;
    mat4 projectionMatrix;
} Camera;

// Function prototypes
void camera_init(Camera* camera, vec3 position, vec3 front, vec3 up);
void camera_updateVectors(Camera* camera);
void camera_updateViewMatrix(Camera* camera);
void camera_updateProjection(Camera* camera, int width, int height);
void camera_rotate(Camera* camera, float yawOffset, float pitchOffset);
void camera_move(Camera* camera, vec3 direction, float speed);
void camera_getViewMatrix(Camera* camera, mat4 viewMatrix);
void camera_getProjectionMatrix(Camera* camera, mat4 projectionMatrix);
void camera_getPosition(Camera* camera, vec3 position);
void camera_getFront(Camera* camera, vec3 front);

#endif // CAMERA_H 