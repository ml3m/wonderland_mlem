#!/bin/bash

# Enchanted Wonderlands - Setup Script
# This script sets up the project directory structure and downloads necessary files

echo "=== Enchanted Wonderlands - Setup Script ==="
echo "This script will set up the project directory structure and download necessary files."
echo ""

# Create directory structure
echo "Creating directory structure..."

# Create main directories
mkdir -p assets/models
mkdir -p assets/textures/terrain
mkdir -p assets/textures/skybox
mkdir -p assets/textures/water
mkdir -p assets/sounds
mkdir -p src/physics
mkdir -p src/rendering
mkdir -p src/scene
mkdir -p src/utils
mkdir -p src/shaders
mkdir -p include/physics
mkdir -p include/rendering
mkdir -p include/scene
mkdir -p include/utils
mkdir -p external/imgui
mkdir -p build
mkdir -p screenshots

# Download STB image library if it doesn't exist
if [ ! -f "external/stb/stb_image.h" ]; then
    echo "Downloading STB image library..."
    curl -o external/stb/stb_image.h https://raw.githubusercontent.com/nothings/stb/master/stb_image.h
fi

# Create basic shader files if they don't exist
if [ ! -d "src/shaders/gbuffer.vert" ]; then
    echo "Creating basic shader files..."
    # Shader files will be created by the user or copied from the provided code
fi

# Create directories for asset files
echo "Creating asset directories..."
mkdir -p assets/textures/terrain/meadow
mkdir -p assets/textures/terrain/forest
mkdir -p assets/textures/terrain/rocky
mkdir -p assets/textures/water
mkdir -p assets/textures/skybox/day
mkdir -p assets/textures/skybox/night
mkdir -p assets/textures/skybox/sunset
mkdir -p assets/models/vegetation
mkdir -p assets/models/structures
mkdir -p assets/models/props

echo ""
echo "=== Setup complete! ==="
echo "Project directory structure has been created."
echo "You can now build the project using:"
echo "./compile_test.sh"
echo ""
echo "Or install dependencies first with:"
echo "./install_dependencies.sh"
echo "" 