#include "utils/shader_loader.h"

// Read file contents
static char* readFile(const char* filePath) {
    FILE* file = fopen(filePath, "r");
    if (!file) {
        fprintf(stderr, "Failed to open file: %s\n", filePath);
        return NULL;
    }
    
    // Get file size
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);
    
    // Allocate memory for file content
    char* content = (char*)malloc(size + 1);
    if (!content) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(file);
        return NULL;
    }
    
    // Read file content
    fread(content, size, 1, file);
    content[size] = '\0';
    
    fclose(file);
    return content;
}

// Compile shader
static GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    
    // Check compilation status
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
        fprintf(stderr, "Shader compilation error: %s\n", infoLog);
        return 0;
    }
    
    return shader;
}

// Load vertex and fragment shaders
GLuint shader_load(const char* vertexPath, const char* fragmentPath) {
    char* vertexSource = readFile(vertexPath);
    char* fragmentSource = readFile(fragmentPath);
    
    if (!vertexSource || !fragmentSource) {
        if (vertexSource) free(vertexSource);
        if (fragmentSource) free(fragmentSource);
        return 0;
    }
    
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
    
    free(vertexSource);
    free(fragmentSource);
    
    if (!vertexShader || !fragmentShader) {
        return 0;
    }
    
    // Create shader program
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    
    // Check linking status
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(program, sizeof(infoLog), NULL, infoLog);
        fprintf(stderr, "Shader program linking error: %s\n", infoLog);
        return 0;
    }
    
    // Clean up
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return program;
}

// Load vertex, geometry, and fragment shaders
GLuint shader_loadWithGeometry(const char* vertexPath, const char* geometryPath, const char* fragmentPath) {
    char* vertexSource = readFile(vertexPath);
    char* geometrySource = readFile(geometryPath);
    char* fragmentSource = readFile(fragmentPath);
    
    if (!vertexSource || !geometrySource || !fragmentSource) {
        if (vertexSource) free(vertexSource);
        if (geometrySource) free(geometrySource);
        if (fragmentSource) free(fragmentSource);
        return 0;
    }
    
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    GLuint geometryShader = compileShader(GL_GEOMETRY_SHADER, geometrySource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
    
    free(vertexSource);
    free(geometrySource);
    free(fragmentSource);
    
    if (!vertexShader || !geometryShader || !fragmentShader) {
        return 0;
    }
    
    // Create shader program
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, geometryShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    
    // Check linking status
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(program, sizeof(infoLog), NULL, infoLog);
        fprintf(stderr, "Shader program linking error: %s\n", infoLog);
        return 0;
    }
    
    // Clean up
    glDeleteShader(vertexShader);
    glDeleteShader(geometryShader);
    glDeleteShader(fragmentShader);
    
    return program;
}

// Load compute shader
GLuint shader_loadCompute(const char* computePath) {
    char* computeSource = readFile(computePath);
    
    if (!computeSource) {
        return 0;
    }
    
    GLuint computeShader = compileShader(GL_COMPUTE_SHADER, computeSource);
    
    free(computeSource);
    
    if (!computeShader) {
        return 0;
    }
    
    // Create shader program
    GLuint program = glCreateProgram();
    glAttachShader(program, computeShader);
    glLinkProgram(program);
    
    // Check linking status
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(program, sizeof(infoLog), NULL, infoLog);
        fprintf(stderr, "Compute shader program linking error: %s\n", infoLog);
        return 0;
    }
    
    // Clean up
    glDeleteShader(computeShader);
    
    return program;
}

// Use shader program
void shader_use(GLuint program) {
    glUseProgram(program);
}

// Set uniform values
void shader_setInt(GLuint program, const char* name, int value) {
    glUniform1i(glGetUniformLocation(program, name), value);
}

void shader_setFloat(GLuint program, const char* name, float value) {
    glUniform1f(glGetUniformLocation(program, name), value);
}

void shader_setVec2(GLuint program, const char* name, float x, float y) {
    glUniform2f(glGetUniformLocation(program, name), x, y);
}

void shader_setVec3(GLuint program, const char* name, float x, float y, float z) {
    glUniform3f(glGetUniformLocation(program, name), x, y, z);
}

void shader_setVec4(GLuint program, const char* name, float x, float y, float z, float w) {
    glUniform4f(glGetUniformLocation(program, name), x, y, z, w);
}

void shader_setMat4(GLuint program, const char* name, const float* value) {
    glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_FALSE, value);
} 