#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>

#include "stb_image.h"
#include "shader.h"
#include "VBO.h"
#include "EBO.h"
#include "VAO.h"
#include "texture.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
GLFWwindow *startupGLFW();


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

//vertex coordinates (pyramid)
GLfloat vertices[] = {
    //     COORDINATES     /        COLORS      /   TexCoord  //
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
};

// indices for vertices order
const int drawOrder[] = {
	0, 1, 2,
	0, 2, 3,
	0, 1, 4,
	1, 2, 4,
	2, 3, 4,
	3, 0, 4
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

    vao1.linkAttrib(vbo1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*) 0);
    vao1.linkAttrib(vbo1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*) (3 * sizeof(float)));
    vao1.linkAttrib(vbo1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*) (6 * sizeof(float)));
    //unbind VAO/VBO/EBO to prevent accidental modification
    vao1.unbind();
    vbo1.unbind();
    ebo1.unbind();

    //initialize textures from given path
    Texture popCat( "../resources/textures/pop_cat.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	popCat.texUnit(myShader, "tex0", 0);

    Texture brick( "../resources/textures/brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    brick.texUnit(myShader, "tex0", 0);

    //rotation rate specification
    float rotation = 0.0f;
    double prevTime = glfwGetTime();

    //enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);   //uncomment to draw in wireframe mode
    //render loop
    while (!glfwWindowShouldClose(window))
    {
        //process user input
        processInput(window);

        //specify background color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //clear color and depth buffers to prevent garbage from being drawnt o screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //tell OpenGL state machine to use previously initialized shader
        myShader.use();

        //increment rotation amount
        double curTime = glfwGetTime();
        if(curTime - prevTime >= 1.0f/60.0f){
            rotation += 0.5f;
            prevTime = curTime;
        }

        //initialize matrices to the identity matrix
        glm::mat4 model = glm::mat4(1.0f);          //model matrix: transforms local coordinates to world coordinates
        glm::mat4 view = glm::mat4(1.0f);           //view matrix: transforms world coordinates to view space
        glm::mat4 projection = glm::mat4(1.0f);     //projection matrix: transforms view space into clip space

        //rotate the object
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
        //move camera away from world coordinate origin
        view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));
        //get perspective
                                        //45 degree FOV     //aspect ratio              //closest   //farthest
        projection = glm::perspective(glm::radians(45.0f), ((float)SCR_WIDTH)/SCR_HEIGHT, 0.1f, 100.0f);

        //send values in the matrices to the vertex shader
        int modelLoc = glGetUniformLocation(myShader.programID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        int viewLoc = glGetUniformLocation(myShader.programID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        int projectionLoc = glGetUniformLocation(myShader.programID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        
        //scale the vertices
        myShader.setFloatUniform("scale", 1.0f);
        //give it a texture
        // popCat.bind();
        brick.bind();

        //bind VAO and draw
        vao1.bind();
        glDrawElements(GL_TRIANGLES, sizeof(drawOrder) / sizeof(int), GL_UNSIGNED_INT, 0);

 
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //deallocate resources
    vao1.destroy();
    vbo1.destroy();
    ebo1.destroy();
    popCat.destroy();
    myShader.destroy();

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