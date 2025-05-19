#!/bin/bash

# Enchanted Wonderlands - Compilation Test Script
# This script attempts to build the project to verify the setup is correct

echo "=== Enchanted Wonderlands - Compilation Test ==="
echo "This script will attempt to build the project to test your setup."
echo ""

# Make sure the external directory contains the required files
if [ ! -f "external/stb/stb_image.h" ]; then
    echo "stb_image.h not found. Downloading..."
    mkdir -p external/stb
    curl -o external/stb/stb_image.h https://raw.githubusercontent.com/nothings/stb/master/stb_image.h
    if [ $? -ne 0 ]; then
        echo "Failed to download stb_image.h. Please download it manually."
        exit 1
    fi
fi

# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
    echo "Creating build directory..."
    mkdir -p build
fi

# Navigate to build directory
cd build

# Run CMake
echo "Running CMake..."
cmake ..
if [ $? -ne 0 ]; then
    echo "CMake configuration failed. Please check the error messages above."
    exit 1
fi

# Build the project
echo "Building the project..."
make -j$(sysctl -n hw.ncpu)
if [ $? -ne 0 ]; then
    echo "Build failed. Please check the error messages above."
    exit 1
fi

echo ""
echo "=== Compilation successful! ==="
echo "You can run the application with:"
echo "./EnchantedWonderlands"
echo ""
echo "If you see any shader compilation errors when running, it's expected as we haven't"
echo "included all the shader files in this template. You would need to create those files."
echo "" 