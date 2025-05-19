#ifndef WONDERLANDS_H
#define WONDERLANDS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

// OpenGL headers
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
// Include GLUT after gl3 to avoid warnings
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#endif

// GLM for matrix mathematics
// We're using C, so we need to use the C-compatible GLM API
#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.h>

// Helper macro for GLM radians
#define glm_rad(degrees) ((degrees) * (3.14159265358979323846f / 180.0f))

// Project headers
#include "config.h"
#include "utils/shader_loader.h"
#include "utils/texture_loader.h"
#include "utils/model_loader.h"
#include "utils/debug.h"
#include "rendering/renderer.h"
#include "rendering/camera.h"
#include "rendering/terrain.h"
#include "rendering/water.h"
#include "rendering/skybox.h"
#include "rendering/particles.h"
#include "physics/fluid_simulation.h"
#include "scene/scene_manager.h"
#include "scene/object.h"

#endif // WONDERLANDS_H 