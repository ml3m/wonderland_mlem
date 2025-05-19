#include "utils/model_loader.h"
#include <stdio.h>
#include <stdlib.h>

// Load a model from a file path
Model* model_load(const char* path) {
    // This is a placeholder implementation
    // In a real implementation, this would use assimp to load the model
    printf("Loading model: %s\n", path);
    
    Model* model = (Model*)malloc(sizeof(Model));
    if (!model) {
        fprintf(stderr, "Failed to allocate memory for model\n");
        return NULL;
    }
    
    // Initialize with empty values
    model->meshes = NULL;
    model->numMeshes = 0;
    model->name = strdup(path);
    model->path = strdup(path);
    
    return model;
}

// Clean up model resources
void model_cleanup(Model* model) {
    if (!model) return;
    
    // Free meshes
    if (model->meshes) {
        for (unsigned int i = 0; i < model->numMeshes; i++) {
            // Delete OpenGL buffers
            if (model->meshes[i].VAO) glDeleteVertexArrays(1, &model->meshes[i].VAO);
            if (model->meshes[i].VBO) glDeleteBuffers(1, &model->meshes[i].VBO);
            if (model->meshes[i].EBO) glDeleteBuffers(1, &model->meshes[i].EBO);
        }
        free(model->meshes);
    }
    
    // Free strings
    if (model->name) free(model->name);
    if (model->path) free(model->path);
    
    // Free model
    free(model);
}

// Render a model
void model_render(Model* model, GLuint shader) {
    if (!model) return;
    
    for (unsigned int i = 0; i < model->numMeshes; i++) {
        model_renderMesh(&model->meshes[i], shader);
    }
}

// Render a single mesh
void model_renderMesh(Mesh* mesh, GLuint shader) {
    if (!mesh || !mesh->VAO) return;
    
    // Bind the VAO
    glBindVertexArray(mesh->VAO);
    
    // Draw elements if we have indices, otherwise draw arrays
    if (mesh->EBO && mesh->numIndices > 0) {
        glDrawElements(GL_TRIANGLES, mesh->numIndices, GL_UNSIGNED_INT, 0);
    } else if (mesh->numVertices > 0) {
        glDrawArrays(GL_TRIANGLES, 0, mesh->numVertices);
    }
    
    // Unbind VAO
    glBindVertexArray(0);
} 