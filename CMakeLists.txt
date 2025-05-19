cmake_minimum_required(VERSION 3.16)
project(EnchantedWonderlands VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# macOS specific settings
if(APPLE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DGL_SILENCE_DEPRECATION")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGL_SILENCE_DEPRECATION")
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64") # Universal binary for Intel and Apple Silicon
endif()

# Find packages
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)

# Include directories
include_directories(
    ${OPENGL_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
    ${ASSIMP_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/utils
    ${CMAKE_SOURCE_DIR}/src/rendering
    ${CMAKE_SOURCE_DIR}/src/physics
    ${CMAKE_SOURCE_DIR}/src/scene
    ${CMAKE_SOURCE_DIR}/external
)

# Add external libraries
add_subdirectory(external)

# Source files
file(GLOB_RECURSE SOURCES 
    "src/*.c"
    "src/utils/*.c"
    "src/rendering/*.c"
    "src/physics/*.c"
    "src/scene/*.c"
    "src/models/*.c"
)

# Create executable
add_executable(EnchantedWonderlands ${SOURCES})

# Find GLUT
if(APPLE)
    # On macOS, use the framework
    find_library(GLUT_LIBRARY GLUT)
    target_link_libraries(EnchantedWonderlands ${GLUT_LIBRARY})
else()
    # On other platforms, use pkg-config or find_package
    find_package(GLUT REQUIRED)
    target_link_libraries(EnchantedWonderlands ${GLUT_LIBRARIES})
endif()

# Link libraries
target_link_libraries(EnchantedWonderlands
    ${OPENGL_LIBRARIES}
    GLEW::GLEW
    ${ASSIMP_LIBRARIES}
    imgui
    stb
)

# macOS specific framework links
if(APPLE)
    target_link_libraries(EnchantedWonderlands "-framework OpenGL")
endif()

# Copy shader and asset files to build directory
file(COPY ${CMAKE_SOURCE_DIR}/src/shaders DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR}) 