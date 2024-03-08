#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>
#include "shader.h"
#include "mytypes.h"

#define MAX_SIERPINSKI_DEPTH 8      //change this to allow for more recursive depth for Sierpinski's Triangle

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void addSierpinskiPts(point_t a, point_t b, point_t c, int depth, std::vector<float>& points);
void initSierpinski(std::vector<float> &points);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;


int main()
{
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
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to load all OpenGL function pointers\n");
        glfwTerminate();
        return -1;
    }    
    //-----------FINISHED INITIALIZING GLFW AND LOADING OPENGL FUNCTION POINTERS-------------------


    //------------------------------------INTIALIZE SHADERS----------------------------------------
    Shader myShader("VertexShader.glsl", "FragmentShader.glsl");
    //------------------------------------DONE INTIALIZING SHADERS---------------------------------
    
    //--------VERTICES FOR THE BACKGROUND----------------
    float rect[] = {
        //position              //color
        -1.0f, -1.0f,  0.5f,    1.0f,  1.0f,  0.5f,     //bottom left
        -1.0f,  1.0f,  1.0f,    1.0f,  1.0f,  0.125f,   //top left
         1.0f,  1.0f, -1.0f,    1.0f,  1.0f,  0.125f,   //top right
         1.0f, -1.0f,  0.5f,    1.0f,  1.0f,  0.5f      //bottom right
    };
    unsigned int drawOrder[] = {
        0, 1, 2,    //triangle one (top left)
        0, 2, 3     //triangle two (bottom right)
    };
    //--------VERTICES FOR THE BACKGROUND----------------

    //---------FILLING VERTICES WITH POINTS OF SIERPINSKI TRIANGLE-----------
    std::vector<float> points = {};
    initSierpinski(points);
    size_t len = points.size();
    float vertices[len];
    //add all elements into array so we can put it in the VBO
    for(int i = 0; i < len; i++){
        vertices[i] = points.at(i);
    }
    //---------FILLING VERTICES WITH POINTS OF SIERPINSKI TRIANGLE-----------

    //VAOs and VBOs for sierpinski triangle
    unsigned int triVAO, triVBO;
    glGenVertexArrays(1, &triVAO);
    glGenBuffers(1, &triVBO);

    glBindVertexArray(triVAO);
    glBindBuffer(GL_ARRAY_BUFFER, triVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //assigning first attribute (position)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //assign second attribute (color);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);



    //VAOs, VBOs, EBOs for the background
    unsigned int bgVAO, bgVBO, bgEBO;
    glGenVertexArrays(1, &bgVAO);
    glGenBuffers(1, &bgVBO);
    glGenBuffers(1, &bgEBO);

    glBindVertexArray(bgVAO);
    glBindBuffer(GL_ARRAY_BUFFER, bgVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bgEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(drawOrder), drawOrder, GL_STATIC_DRAW);
    

    glBindBuffer(GL_ARRAY_BUFFER, 0);   //unbind VBOs
    glBindVertexArray(0);       //unbind VAOs

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);   //uncomment to draw in wireframe mode
    //render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        //fill background
        // -----------------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //use shader programs defined in shader.h
        myShader.use();
        //render background rectangle
        glBindVertexArray(bgVAO);
        glBindBuffer(GL_ARRAY_BUFFER, bgVBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        //render sierpinski triangle
        glBindVertexArray(triVAO);
        int nBufferSize = 0;
        glBindBuffer(GL_ARRAY_BUFFER, triVBO);
        glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &nBufferSize);
        for(int i = 0; i < nBufferSize; i+= 3){
            glDrawArrays(GL_TRIANGLES, i, 3);
        }


        
 
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //deallocate resources
    glDeleteVertexArrays(1, &triVAO);
    glDeleteBuffers(1, &triVBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}


void initSierpinski(std::vector<float> &points){
    point_t a = {-0.5f, -0.5f,  0.0f};
    point_t b = { 0.0f,  0.5f,  0.0f};
    point_t c = { 0.5f, -0.5f,  0.0f};
    addSierpinskiPts(a, b, c, 0, points);
}


/**
 * Helper method to calculate midpoint between two coordinates in 3d space
*/
point_t midpoint(point_t a, point_t b){
    point_t p;
    p.x = (a.x + b.x) / 2.0;
    p.y = (a.y + b.y) / 2.0;
    p.z = (a.z + b.z) / 2.0;
    return p;
}

/**
 * Helper function that adds all the position and color values into a vector of floats
*/
void addPosAndColor(point_t a, point_t b, point_t c, int depth, std::vector<float> &points){
        points.push_back(a.x);
        points.push_back(a.y);
        points.push_back(a.z);
        points.push_back(0.25f);
        points.push_back(depth * 1.0/MAX_SIERPINSKI_DEPTH);
        points.push_back(0.75f);

        points.push_back(b.x);
        points.push_back(b.y);
        points.push_back(b.z);
        points.push_back(0.25f);
        points.push_back(depth * 1.0/MAX_SIERPINSKI_DEPTH);
        points.push_back(0.75f);


        points.push_back(c.x);
        points.push_back(c.y);
        points.push_back(c.z);
        points.push_back(0.25f);
        points.push_back(depth * 1.0/MAX_SIERPINSKI_DEPTH);
        points.push_back(0.75f);
}
/**
 * Recursively puts the info needed to draw a sierpinski triangle into
 * a vector
 * @param a a 3d point representing one of the vertices of a triangle
 * @param b another vertex of same triangle
 * @param c another vertex of same triangle
 * @param depth the current recursive depth
 * @param points the vector we want to add the points to
 * 
 * post: points should have the points of a sierpinski triangle, with the smallest recursive triangle 
 *       held at the end of the vector
 */
void addSierpinskiPts(point_t a, point_t b, point_t c, int depth, std::vector<float>& points){
    if(depth <= MAX_SIERPINSKI_DEPTH){
        addPosAndColor(a, b, c, depth, points);

        point_t ab = midpoint(a, b);
        point_t ac = midpoint(a, c);
        point_t bc = midpoint(b, c);
        addSierpinskiPts(a, ab, ac, depth + 1, points);
        addSierpinskiPts(b, ab, bc, depth + 1, points);
        addSierpinskiPts(c, ac, bc, depth + 1, points);
    }
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