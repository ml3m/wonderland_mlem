#ifndef DEBUG_H
#define DEBUG_H

#include "wonderlands.h"

// Debug message types
typedef enum {
    DEBUG_INFO,
    DEBUG_WARNING,
    DEBUG_ERROR
} DebugMessageType;

// Function prototypes
void debug_init();
void debug_cleanup();
void debug_log(DebugMessageType type, const char* message);
void debug_logf(DebugMessageType type, const char* format, ...);
void debug_showImGuiWindow(bool* show);
void debug_renderOverlay();
void debug_checkGLError(const char* operation);

#endif // DEBUG_H 