#!/bin/bash

# Enchanted Wonderlands - Dependency Installation Script
# This script installs all the required dependencies for building and running the project.

echo "=== Enchanted Wonderlands - Dependency Installation Script ==="
echo "This script will install all the required dependencies for macOS."
echo ""

# Check if Homebrew is installed
if ! command -v brew &> /dev/null; then
    echo "Homebrew is not installed. Installing Homebrew..."
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    
    # Check if Homebrew installation was successful
    if ! command -v brew &> /dev/null; then
        echo "Failed to install Homebrew. Please install it manually from https://brew.sh/"
        exit 1
    fi
else
    echo "Homebrew is already installed. Updating..."
    brew update
fi

# Install CMake if not already installed
if ! command -v cmake &> /dev/null; then
    echo "Installing CMake..."
    brew install cmake
else
    echo "CMake is already installed."
fi

# Install required libraries
echo "Installing required libraries..."
brew install glew glm assimp

# Check for GLUT
echo "Checking for GLUT..."
if [ ! -f "/System/Library/Frameworks/GLUT.framework/GLUT" ]; then
    echo "GLUT framework not found. It should be included with macOS."
    echo "If you're having issues, try installing freeglut:"
    echo "brew install freeglut"
else
    echo "GLUT framework found."
fi

# Create build directory
echo "Creating build directory..."
mkdir -p build

echo "=== Installation complete! ==="
echo "You can now build the project by running:"
echo "cd build"
echo "cmake .."
echo "make"
echo ""
echo "Then run the application with:"
echo "./EnchantedWonderlands"
echo ""

# Make the script executable
chmod +x ./install_dependencies.sh 