#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "shader.h"
#include "VBO.h"
#include "EBO.h"
#include "VAO.h"

typedef struct {
    int width;
    int height;
    int channels;
    unsigned char *bytes;
} image_t;


void loadImage(image_t *img, const char *path);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
GLFWwindow *startupGLFW();


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;
const float vertices[] = {
        //position              //color
         0.5f,  0.5f, 0.0f,     0.0f, .25f, .25f,      // top right
         0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 1.0f,      // bottom right
        -0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 1.0f,      // bottom left
        -0.5f,  0.5f, 0.0f,     0.0f, .25f, .25f       // top left 
};

const int drawOrder[] = {
        0, 1, 3,  // first triangle
        1, 2, 3   // second triangle
};

int main()
{
    //initialize window
    GLFWwindow* window = startupGLFW();
    if(!window){
        glfwTerminate();
        return -1;
    }
    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to load all OpenGL function pointers\n");
        glfwTerminate();
        return -1;
    }    

    //get shader program from path specified
    Shader myShader("../resources/shaders/VertexShader.glsl", "../resources/shaders/FragmentShader.glsl");
    
    VertArrObj vao1;
    vao1.bind();

    VertBufObj vbo1((float *)vertices, sizeof(vertices), GL_STATIC_DRAW);
    ElemBufObj ebo1((int *)drawOrder, sizeof(drawOrder), GL_STATIC_DRAW);

    vao1.linkAttrib(vbo1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*) 0);
    vao1.linkAttrib(vbo1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    vao1.unbind();
    vbo1.unbind();
    ebo1.unbind();


    //load texture image
    // image_t texture;
    // loadImage(&texture, "resources/textures/pop_cat.png");

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);   //uncomment to draw in wireframe mode
    //render loop
    while (!glfwWindowShouldClose(window))
    {
        //process user input
        processInput(window);

        //fill background
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //use shader programs defined in shader.h
        myShader.use();
        myShader.setFloatUniform("scale", 0.5f);
        //bind VAO and draw
        vao1.bind();

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

 
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //deallocate resources

    vao1.destroy();
    vbo1.destroy();
    ebo1.destroy();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

/**
 * function that loads an image with desired width/height/No. channels
 * @param img pointer to the image we want to initialize
 * @param path the path to the image that we want to load
*/
void loadImage(image_t *img, const char *path){
    //loads the byte array with image data and then fills the remaining fields
    img->bytes = stbi_load(path, &img->width, &img->height, &img->channels, 0);
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

/**
 * Function that initializes a glfw window and makes it the active window
 * @return a pointer to the initialized window. return NULL if the window failed
 *         to initialize.
*/
GLFWwindow *startupGLFW(){

    // glfw: initialize to context version 3.3
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    //-----------INTIIALIZE GLFW AND LOAD OPENGL FUNCTION POINTERS---------------------------------

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        printf("Failed to init window\n");
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    return window;

}