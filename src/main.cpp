#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>
#include "shader.h"
#include "mytypes.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void drawSierpinski(point_t a, point_t b, point_t c, int depth);
void addSierpinskiPts(point_t a, point_t b, point_t c, int depth, std::vector<point_t>& points);
void loadSierpinskiIntoBuffer(std::vector<point_t>& points, unsigned int VAO, unsigned int VBO);


// settings
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

    float firstTriangle[] = {
        //positions           //colors
         0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,   //bottom right
        -0.5f, -0.5f,  0.0f,  0.0f,  1.0f,  0.0f,   //bottom left
         0.0f,  0.5f,  0.0f,  0.0f,  0.0f,  1.0f    //top mid
    };

    //use this rect to cover entire screen
    float rect[] = {
        -1.0f, -1.0f,  0.5f,    //bottom left
        -1.0f,  1.0f,  1.0f,    //top left
         1.0f,  1.0f, -1.0f,    //top right
         1.0f, -1.0f,  0.5f     //bottom right
    };
    unsigned int drawOrder[] = {
        0, 1, 2,    //triangle one (top left)
        0, 2, 3     //triangle two (bottom right)
    };


    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //set up info for rendering

    //bind vertex array object
    glBindVertexArray(VAO);

    //copy vertices array into vertex buffer object for openGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(drawOrder), drawOrder, GL_STATIC_DRAW);

    //set vertex attribute pointers for position attribute (rectangle)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);



    std::vector<point_t> points = {};
    point_t a = {-0.5f, -0.5f,  0.0f};
    point_t b = { 0.0f,  0.5f,  0.0f};
    point_t c = { 0.5f, -0.5f,  0.0f};
    int depth = 5;

    addSierpinskiPts(a, b, c, depth, points);

    unsigned int sierpinskiVBO, sierpinskiVAO;
    glGenVertexArrays(1, &sierpinskiVAO);
    glGenBuffers(1, &sierpinskiVBO);

    loadSierpinskiIntoBuffer(points, sierpinskiVAO, sierpinskiVBO);
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

        //use shader programs defined in our class
        myShader.use();
        //render background rectangle
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        //render sierpinski triangle
        glBindVertexArray(sierpinskiVAO);

        int nBufferSize = 0;
        glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &nBufferSize);
        for(int i = 0; i < nBufferSize; i += 3){
            glDrawArrays(GL_TRIANGLES, i, 3);
        }

        
 
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //deallocate resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
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
void addSierpinskiPts(point_t a, point_t b, point_t c, int depth, std::vector<point_t>& points){
    if(depth > 0){
        points.push_back(a);
        points.push_back(b);
        points.push_back(c);

        point_t ab = midpoint(a, b);
        point_t ac = midpoint(a, c);
        point_t bc = midpoint(b, c);
        addSierpinskiPts(ab, ac, bc, depth - 1, points);
    }
}
/**
 * pre: VBO is a valid id for a vertex buffer object that has been generated
 *      (i.e. glGenBuffers(1, &VBO) has been called)
 * 
*/
void loadSierpinskiIntoBuffer(std::vector<point_t>& points, unsigned int VAO, unsigned int VBO){
    size_t len = points.size();
    float vertices[len * 3];
    //add all elements into array so we can put it in the VBO
    for(int i = 0; i < len; i++){
        vertices[3 * i] = points.at(i).x;
        vertices[3 * i + 1] = points.at(i).y;
        vertices[3 * i + 2] = points.at(i).z;
    }

    //store old buffer binding
    int oldBufferBinding;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &oldBufferBinding);

    //store old VAO
    int oldVAO;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &oldVAO);

    //bind new VAO to sierpinski points
    glBindVertexArray(VAO);

    //insert sierpinski points into VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);   //TODO: see if putting this here works
    


    //restore old buffer binding
    glBindBuffer(GL_ARRAY_BUFFER, oldBufferBinding);
    //restore old VAO binding
    glBindVertexArray(oldVAO);

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