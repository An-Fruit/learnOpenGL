/**
 * Class used to manage a shader program
 * 
*/
#ifndef SHADER_H
#define SHADER_H

#include<glad/glad.h>   //get required OpenGL headers

#include<iostream>      //I/O libraries needed to read Vertex & Fragment Shader Files
#include<stdio.h>
#include<fstream>
#include<sstream>
#include<string>

#define SHADER_PROGRAM 0xDEADBEEF

class Shader{
    public:
        unsigned int programID;    //program ID

        /**
         * Constructor for a Shader object
         * @param vShaderPath the path to the vertex shader
         * @param fShaderPath the path to the fragment shader
         * pre: none
         * post: Shader object constructed with a program ID referring to 
         *       a shader program that has linked the vertex and fragment
         *       shaders specified in the paths given to the constructor.
        */
        Shader(const char *vShaderPath, const char *fShaderPath){
            //1. retrieve source code from path(s)
            std::string vertexCode = getFileContents(vShaderPath);
            std::string fragmentCode = getFileContents(fShaderPath);

            //convert to C-style strings since
            //openGL only recognizes them as valid shader programs
            const char *vShaderSourceCode = vertexCode.c_str();
            const char *fShaderSourceCode = fragmentCode.c_str();

            //2. compile and link shaders
            unsigned int vert, frag;
            //compile vertex shader
            vert = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vert, 1, &vShaderSourceCode, NULL);
            glCompileShader(vert);
            checkError(vert, GL_VERTEX_SHADER);

            //compile fragment shader
            frag = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(frag, 1, &fShaderSourceCode, NULL);
            glCompileShader(frag);
            checkError(frag, GL_FRAGMENT_SHADER);


            //link shaders
            programID = glCreateProgram();
            glAttachShader(programID, vert);
            glAttachShader(programID, frag);
            glLinkProgram(programID);
            checkError(programID, SHADER_PROGRAM);

            //delete unused shaders
            glDeleteShader(vert);
            glDeleteShader(frag);
        }
        
        
        /**
         * @param vPath the path to the file w/ vertex shader source code
         * @param fPath the path to a file w/ fragment shader source code
         * pre: vPath & fPath are valid directories
         * post: this shader object now refers to a shader program linking
         *       the shaders specified by the given paths
        */
        void setShaders(const char *vPath, const char *fPath){
            *this = Shader(vPath, fPath);
        }

        /**
         * Causes the OpenGL state machine to use the shader program defined
         * in this object to render
         * pre: none
         * post: OpenGL state machine uses the shader program defined in
         *       this object to render
        */
        void use(){                
            glUseProgram(programID);
        }

        //modifier methods to set uniform shader attributes
        /**
         * @param name the name of the uniform attribute we want to set
         * @param value the value we want to change the uniform attribute to 
        */
        void setBool(const std::string &name, bool value) const{
            glUniform1i(glGetUniformLocation(programID, name.c_str()), (int)value); 
        }

        /**
         * @param name the name of the uniform attribute we want to set
         * @param value the value we want to change the uniform attribute to 
        */
        void setInt(const std::string &name, int value) const{
            glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
        }

        /**
         * @param name the name of the uniform attribute we want to set
         * @param value the value we want to change the uniform attribute to 
        */
        void setFloat(const std::string &name, float value) const{
            glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
        }

    private:

        /**
         * private helper function that takes a path to a file, and returns the contents
         * of that file in a std::string
         * @param path a string that specifies the path to a file
         * @return a std::string that contains the contents of the file
         * pre: path is a valid path to a file
         * post: return the contents of that file inside a std::string object
        */
        std::string getFileContents(const char *path){
            std::ifstream file;
            std::string fContent;
            //make sure we can throw exceptions
            file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            try{
                file.open(path);
                std::stringstream fileContentStream;
                fileContentStream << file.rdbuf();

                file.close();

                fContent = fileContentStream.str();

            } catch(std::ifstream::failure error){
                printf("\nERROR TRYING TO READ IN FILE");
            }
            return fContent;
        }

        /**
         * private helper function to check for compilation and linking errors
         * @param shader an ID to a valid shader or shader program
         * @param shaderType the type of shader passed into the function
         * pre: shaderType == GL_VERTEX_SHADER || GL_FRAGMENT_SHADER || SHADER_PROGRAM
         * post: checks for errors in compilation/linking process
        */
        void checkError(unsigned int shader, unsigned int shaderType){
            int success;
            char log[1024];
            switch(shaderType){
                //check error for shader compilation
                case GL_VERTEX_SHADER:
                case GL_FRAGMENT_SHADER:
                    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
                    if(!success){
                        glGetShaderInfoLog(shader, 1024, NULL, log);
                        printf("\nSHADER COMPILATION ERROR:\n%s\n", log);
                    }
                    break;

                //check error for shader linking
                case SHADER_PROGRAM:
                    glGetProgramiv(shader, GL_LINK_STATUS, &success);
                    if(!success){
                        glGetProgramInfoLog(shader, 1024, NULL, log);
                        printf("\n SHADER LINKING ERROR:\n%s\n", log);
                    }
                    break;

                default:
                    printf("\nCHECK_ERROR called for undefined type\n");
                    break;
            }
        }

};

#endif