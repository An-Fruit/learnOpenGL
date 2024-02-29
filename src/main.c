#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<stdio.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

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

    const char *vertexShaderSource =    "#version 330 core\n"
                                        "layout (location = 0) in vec3 aPos;\n"
                                        "void main(){\n"
                                        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                        "}\0";

    const char *fragmentShaderSource =  "#version 330 core\n"
                                        "out vec4 FragColor;\n"
                                        "void main(){\n"
                                        "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                        "}\0";

    
    //compile and link vertex and fragment shader
    int success;
    char infoLog[512];
    

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

    //delete the unused shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // all intialization of window/context and shaders completed

    float vertices[] = {        //triangle 1
        -0.5f,  0.5f,  0.0f,    //center top
        -1.0f, -0.5f,  0.0f,    //bottom left
         0.0f, -0.5f,  0.0f,    //bottom right

                                //triangle 2
         0.5f,  0.5f, 0.0f,     //center top
         0.0f, -0.25f, 0.0f,     //bottom left
         1.0f, -0.25f, 0.0f      //bottom right
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //unbind GL_ARRAY_BUFFER to 
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);


    //render loop
    while (!glfwWindowShouldClose(window))
    {
        //get user input
        processInput(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, .5f);
        glClear(GL_COLOR_BUFFER_BIT);

        //draw 2 triangles
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        // glPolygonMode(GL_FRONT, GL_LINE); //wireframe mode
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //deallocate all glfw resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
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