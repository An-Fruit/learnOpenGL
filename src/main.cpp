#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

//shader source code
    const char *vertexShaderSource =          "#version 330 core\n"
                                        "layout (location = 0) in vec3 aPos;\n"
                                        "void main(){\n"
                                        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                        "}\0";

    const char *fragmentShaderSource =  "#version 330 core\n"
                                        "out vec4 FragColor;\n"
                                        "void main(){\n"
                                        "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                        "}\0";
    const char *yellowFragShaderSource =    "#version 330 core\n"
                                            "out vec4 FragColor;\n"
                                            "void main(){\n"
                                            "FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
                                            "}\0";

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
    
    //compile and link vertex and fragment shader
    int success;
    char infoLog[512];
    
    if(!vertexShaderSource){
        printf("FAILED TO READ IN VERTEX SHADER SRC CODE");
        glfwTerminate();
        return -1;
    }
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("ERROR: FAILED TO COMPILE VERTEX SHADER\n%s", infoLog);
        glfwTerminate();
        return -1;
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("ERROR: FAILED TO COMPILE FRAGMENT SHADER\n%s", infoLog);
        glfwTerminate();
        return -1;
    }

    unsigned int yellowFragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(yellowFragShader, 1, &yellowFragShaderSource, NULL);
    glCompileShader(yellowFragShader);
    glGetShaderiv(yellowFragShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(yellowFragShader, 512, NULL, infoLog);
        printf("ERROR: FAILED TO COMPILE YELLOW FRAGMENT SHADER\n%s", infoLog);
        glfwTerminate();
        return -1;
    }

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("ERROR: SHADER PROGRAM LINKING FAILED");
        glfwTerminate();
        return -1;
    }

    unsigned int yellowShaderProg = glCreateProgram();
    glAttachShader(yellowShaderProg, vertexShader);
    glAttachShader(yellowShaderProg, yellowFragShader);
    glLinkProgram(yellowShaderProg);
    glGetProgramiv(yellowShaderProg, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(yellowShaderProg, 512, NULL, infoLog);
        printf("ERROR: YELLOW SHADER PROGRAM LINKING FAILED");
        glfwTerminate();
        return -1;
    }

    //delete the unused shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(yellowFragShader);

    // all intialization of window/context and shaders completed

    float firstTriangle[] = {        //triangle 1
        -0.5f,  0.5f,  0.0f,    //center top
        -1.0f, -0.5f,  0.0f,    //bottom left
         0.0f, -0.5f,  0.0f,    //bottom right
    };

    float secondTriangle[] = {        //triangle 2
         0.5f,  0.5f,  0.0f,     //center top
         0.0f, -0.25f, 0.0f,     //bottom left
         1.0f, -0.25f, 0.0f      //bottom right
    };

    unsigned int VBOs[2], VAOs[2];
    glGenVertexArrays(2, VAOs); // we can also generate multiple VAOs or buffers at the same time
    glGenBuffers(2, VBOs);
    // first triangle setup
    // --------------------
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	// Vertex attributes stay the same
    glEnableVertexAttribArray(0);
    // glBindVertexArray(0); // no need to unbind at all as we directly bind a different VAO the next few lines
    // second triangle setup
    // ---------------------
    glBindVertexArray(VAOs[1]);	// note that we bind to a different VAO now
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);	// and a different VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);   // because the vertex data is tightly packed we can
                                                                    // also specify 0 as the vertex attribute's stride to let
                                                                    // OpenGL figure it out
    glEnableVertexAttribArray(0);
    // glBindVertexArray(0);    // not really necessary as well, but beware of calls that could
                                // affect VAOs while this one is bound 
                                //(like binding element buffer objects, or enabling/disabling vertex attributes)




    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);   //uncomment to draw in wireframe mode
    //render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        // draw first triangle using the data from the first VAO
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // then we draw the second triangle using the data from the second VAO
        glUseProgram(yellowShaderProg);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
 
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //deallocate all glfw resources
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glDeleteProgram(shaderProgram);


    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
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