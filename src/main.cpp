#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>

#include "shader.h"
#include "VBO.h"
#include "EBO.h"
#include "VAO.h"

#define MAX_SIERPINSKI_DEPTH 8      //change this to allow for more recursive depth for Sierpinski's Triangle

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
GLFWwindow *startupGLFW();


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;
const float vertices[] = {
        //position              //color
         0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 1.0f,      // top right
         0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 1.0f,      // bottom right
        -0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 1.0f,      // bottom left
        -0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 1.0f       // top left 
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
    Shader myShader("VertexShader.glsl", "FragmentShader.glsl");
    
    VertArrObj vao1;
    vao1.bind();

    VertBufObj vbo1((float *)vertices, sizeof(vertices), GL_STATIC_DRAW);
    ElemBufObj ebo1((int *)drawOrder, sizeof(drawOrder), GL_STATIC_DRAW);

    vao1.linkAttrib(vbo1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*) 0);
    vao1.linkAttrib(vbo1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    vao1.unbind();
    vbo1.unbind();
    ebo1.unbind();



    // unsigned int VBO, VAO, EBO;
    // glGenVertexArrays(1, &VAO);
    // glGenBuffers(1, &VBO);
    // glGenBuffers(1, &EBO);
    // // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    // glBindVertexArray(VAO);

    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(drawOrder), drawOrder, GL_STATIC_DRAW);
    // // position attribute
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);
    // // color attribute
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    // glEnableVertexAttribArray(1);

    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindVertexArray(0);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);   //unbind EBO after unbinding VAO to prevent them from unbinding from each other

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