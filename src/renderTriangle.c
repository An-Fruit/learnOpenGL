#include <glad/glad.h>
#include <GLFW/glfw3.h>
void drawTriangle(){
    float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
    };  
    unsigned int VBO;
    glGenBuffers(1, &VBO);
}