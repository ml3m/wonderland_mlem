# Enchanted Wonderlands Project Structure

This document describes the structure of the Enchanted Wonderlands project to help you navigate and understand the codebase.

## Directory Structure

```
enchanted-wonderlands/
├── assets/               # Textures, models, and other assets
│   ├── models/           # 3D models
│   ├── textures/         # Texture files
│   │   ├── terrain/      # Terrain textures
│   │   ├── skybox/       # Skybox textures
│   │   └── water/        # Water textures
│   └── sounds/           # Sound effects and music (optional)
│
├── external/             # External libraries
│   ├── imgui/            # Dear ImGui for debug UI
│   └── stb/              # stb libraries for image loading
│
├── include/              # Header files
│   ├── physics/          # Physics system headers
│   │   └── fluid_simulation.h
│   ├── rendering/        # Rendering system headers
│   │   ├── camera.h
│   │   ├── particles.h
│   │   ├── renderer.h
│   │   ├── skybox.h
│   │   ├── terrain.h
│   │   └── water.h
│   ├── scene/            # Scene management headers
│   │   ├── object.h
│   │   └── scene_manager.h
│   ├── utils/            # Utility headers
│   │   ├── debug.h
│   │   ├── model_loader.h
│   │   ├── shader_loader.h
│   │   └── texture_loader.h
│   ├── config.h          # Global configuration
│   └── wonderlands.h     # Main header
│
├── src/                  # Source files
│   ├── physics/          # Physics implementation
│   │   └── fluid_simulation.c
│   ├── rendering/        # Rendering implementation
│   │   ├── camera.c
│   │   ├── particles.c
│   │   ├── renderer.c
│   │   ├── skybox.c
│   │   ├── terrain.c
│   │   └── water.c
│   ├── scene/            # Scene management implementation
│   │   ├── object.c
│   │   └── scene_manager.c
│   ├── shaders/          # GLSL shaders
│   │   ├── blur.frag/vert
│   │   ├── gbuffer.frag/vert
│   │   ├── lighting.frag/vert
│   │   ├── particle.frag/vert
│   │   ├── skybox.frag/vert
│   │   ├── terrain.frag/vert
│   │   └── water.frag/vert
│   ├── utils/            # Utility implementation
│   │   ├── debug.c
│   │   ├── model_loader.c
│   │   ├── shader_loader.c
│   │   └── texture_loader.c
│   └── main.c            # Entry point
│
├── build/                # Build directory (created by CMake)
├── screenshots/          # Screenshots for documentation
├── CMakeLists.txt        # CMake configuration
├── LICENSE               # License file
├── README.md             # Project documentation
├── .gitignore            # Git ignore file
├── install_dependencies.sh # Dependency installation script
└── compile_test.sh       # Compilation test script
```

## Core Components

### Main Components

1. **Main (main.c)**: Entry point of the application, handles GLUT initialization, event handling, and the main loop.

2. **Renderer (renderer.h/c)**: Manages the rendering pipeline, including deferred shading, shadow mapping, and post-processing.

3. **Scene Manager (scene_manager.h/c)**: Manages the scene graph, object placement, and scene updates.

4. **Camera (camera.h/c)**: Handles camera movement, projection, and view matrices.

### Environment Components

1. **Terrain (terrain.h/c)**: Procedural terrain generation with LOD and biome blending.

2. **Water (water.h/c)**: Advanced water simulation with reflections, refractions, and fluid dynamics.

3. **Skybox (skybox.h/c)**: Dynamic sky rendering with day/night cycle and weather effects.

4. **Particles (particles.h/c)**: Particle system for effects like dust, rain, leaves, and fireflies.

### Physics Components

1. **Fluid Simulation (fluid_simulation.h/c)**: Grid-based fluid simulation for realistic water flow.

### Utility Components

1. **Shader Loader (shader_loader.h/c)**: Utility for loading and compiling GLSL shaders.

2. **Texture Loader (texture_loader.h/c)**: Utility for loading and managing textures.

3. **Model Loader (model_loader.h/c)**: Utility for loading 3D models using assimp.

4. **Debug (debug.h/c)**: Debugging utilities and ImGui integration.

## Extending the Project

When adding new features to the project, follow these guidelines:

1. Place header files in the appropriate subdirectory of `include/`
2. Place implementation files in the corresponding subdirectory of `src/`
3. Add new shaders to `src/shaders/`
4. Add new assets to the appropriate subdirectory of `assets/`
5. Update CMakeLists.txt if adding new source files or directories

## Build System

The project uses CMake as its build system. The main CMakeLists.txt file defines the project structure, dependencies, and build targets. Additional CMakeLists.txt files in the `external/` subdirectories configure the external libraries. 