#include "utils/debug.h"
#include <stdio.h>
#include <stdarg.h>

// Initialize debug system
void debug_init() {
    // In a real implementation, this would initialize ImGui and other debug tools
    printf("Debug system initialized\n");
}

// Clean up debug system
void debug_cleanup() {
    // Clean up resources
    printf("Debug system cleaned up\n");
}

// Log a debug message
void debug_log(DebugMessageType type, const char* message) {
    const char* typeStr = "";
    switch (type) {
        case DEBUG_INFO:    typeStr = "INFO"; break;
        case DEBUG_WARNING: typeStr = "WARNING"; break;
        case DEBUG_ERROR:   typeStr = "ERROR"; break;
    }
    
    printf("[%s] %s\n", typeStr, message);
}

// Log a formatted debug message
void debug_logf(DebugMessageType type, const char* format, ...) {
    const char* typeStr = "";
    switch (type) {
        case DEBUG_INFO:    typeStr = "INFO"; break;
        case DEBUG_WARNING: typeStr = "WARNING"; break;
        case DEBUG_ERROR:   typeStr = "ERROR"; break;
    }
    
    printf("[%s] ", typeStr);
    
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    
    printf("\n");
}

// Show ImGui debug window
void debug_showImGuiWindow(bool* show) {
    // This is a placeholder for ImGui implementation
    if (*show) {
        printf("Debug window is shown\n");
    }
}

// Render debug overlay
void debug_renderOverlay() {
    // This is a placeholder for rendering debug information
    // In a real implementation, this would render ImGui windows, etc.
}

// Check for OpenGL errors
void debug_checkGLError(const char* operation) {
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        const char* errorStr = "";
        switch (error) {
            case GL_INVALID_ENUM:      errorStr = "GL_INVALID_ENUM"; break;
            case GL_INVALID_VALUE:     errorStr = "GL_INVALID_VALUE"; break;
            case GL_INVALID_OPERATION: errorStr = "GL_INVALID_OPERATION"; break;
            case GL_OUT_OF_MEMORY:     errorStr = "GL_OUT_OF_MEMORY"; break;
            default:                   errorStr = "Unknown error"; break;
        }
        
        printf("OpenGL error after %s: %s (0x%x)\n", operation, errorStr, error);
    }
} 