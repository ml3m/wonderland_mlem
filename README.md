# Enchanted Wonderlands

A visually stunning 3D wonderlands environment created using C, GLUT, and modern OpenGL techniques optimized for macOS Silicon.

![Enchanted Wonderlands](screenshots/wonderlands.png)

## Project Overview

Enchanted Wonderlands is an interactive 3D environment showcasing advanced graphics programming techniques while maintaining compatibility with Apple Silicon architecture. The project features:

- Procedurally generated terrain with varied biomes (meadow, forest, rocky outcrops)
- Realistic water simulation with reflections, refractions, and dynamic interactions
- Procedurally placed vegetation with wind animation
- Whimsical structures: cottages, bridges, ruins, and magical elements
- Dynamic time-of-day system with realistic lighting
- Particle systems for atmospheric effects
- Weather effects: rain, fog, and cloud systems
- Physically-based rendering for realistic materials

## Technical Features

- Modern OpenGL (4.1+) with proper abstractions
- Deferred rendering pipeline for efficient lighting
- Screen Space Ambient Occlusion (SSAO)
- Shadow mapping for dynamic shadows
- Normal mapping for detailed surfaces
- Instanced rendering for vegetation
- Fluid simulation using compute shaders
- Post-processing effects (bloom, depth of field, tone mapping)

## Build Instructions

### Prerequisites

To build and run Enchanted Wonderlands, you need:

- macOS 11.0+ (Big Sur or later)
- Xcode Command Line Tools
- CMake 3.16+
- Homebrew (recommended for installing dependencies)

### Installing Dependencies

```bash
# Install Homebrew if you don't have it
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install required libraries
brew install cmake glew glm assimp glfw
```

### Building the Project

```bash
# Clone the repository
git clone https://github.com/yourusername/enchanted-wonderlands.git
cd enchanted-wonderlands

# Create build directory
mkdir build && cd build

# Run CMake
cmake ..

# Build the project
make -j8

# Run the application
./EnchantedWonderlands
```

## Controls

- **WASD**: Move camera
- **Mouse**: Look around
- **Q/E**: Move up/down
- **P**: Pause time
- **[/]**: Decrease/increase time speed
- **C/R/F**: Toggle Clear/Rain/Fog weather
- **L**: Toggle wireframe mode
- **ESC**: Exit the application

## Performance Considerations

The application is optimized for Apple Silicon, but for the best experience:

- Enable advanced effects only on high-performance hardware
- Adjust the window size for better performance on older machines
- Reduce the terrain size or LOD levels if experiencing low framerates

## Troubleshooting

### Common Issues

1. **OpenGL/GLUT errors**: Make sure you have the latest graphics drivers
2. **CMake cannot find packages**: Verify that all dependencies are correctly installed
3. **Performance issues**: Try reducing the terrain size or disabling advanced effects

### Support

For issues, questions, or contributions, please open an issue on the GitHub repository or contact the developer.

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- Assets courtesy of various free 3D model repositories
- Terrain generation algorithms inspired by academic papers
- Fluid simulation based on Jos Stam's "Stable Fluids" approach
- PBR implementation based on Epic Games' research
