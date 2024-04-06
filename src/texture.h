#ifndef TEXTURE_CLASS
#define TEXTURE_CLASS

#include<glad/glad.h>
#include "stb_image.h"
#include "shader.h"

class Texture {
    public:
	
    GLuint ID;
	GLenum type;

    /**
     * Constructor for a texture object
     * @param imagePath the path to the image
     * @param texType the type of texture (i.e. 2D, 3D, cubemap, etc.)
     * @param slot the texture slot we want to use (GL_TEXTURE0 by default)
     * @param format the format the texture is stored in (i.e. GL_RGBA)
     * @param pixelType the format the image is stored in (GL_UNSIGNED_BYTE for an unsigned byte array in this case)
    */
	Texture(const char* imagePath, GLenum texType, GLenum slot, GLenum format, GLenum pixelType){
        // Assigns the type of the texture ot the texture object
        type = texType;

        // Stores the width, height, and the number of color channels of the image
        int imgW, imgH, imgCh;
        // Flips the image so it appears right side up
        stbi_set_flip_vertically_on_load(true);
        // Reads the image from a file and stores it in bytes
        unsigned char* bytes = stbi_load(imagePath, &imgW, &imgH, &imgCh, 0);

        // Generates an OpenGL texture object
        glGenTextures(1, &ID);
        // Assigns the texture to a Texture Unit
        glActiveTexture(slot);
        glBindTexture(texType, ID);

        // Configures the type of algorithm that is used to make the image smaller or bigger
        glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // Configures the way the texture repeats (if it does at all)
        glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Extra lines in case you choose to use GL_CLAMP_TO_BORDER
        // float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
        // glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);

        // Assigns the image to the OpenGL Texture object
        glTexImage2D(texType, 0, GL_RGBA, imgW, imgH, 0, format, pixelType, bytes);
        // Generates MipMaps
        glGenerateMipmap(texType);

        // Deletes the image data as it is already in the OpenGL Texture object
        stbi_image_free(bytes);

        // Unbinds the OpenGL Texture object so that it can't accidentally be modified
        glBindTexture(texType, 0);
    }

    /**
     * Assigns a texture unit to a texture
     * @param uniName the name of the uniform
     * @param unit the value we want to set th
    */
	void texUnit(Shader& shader, const char* uniName, GLuint unit){
        // Gets the location of the uniform
        GLuint texUniLoc = glGetUniformLocation(shader.programID, uniName);
        // Shader needs to be activated before changing the value of a uniform
        shader.use();
        // Sets the value of the uniform
        glUniform1i(texUniLoc, unit);
    }

	// Binds a texture
	void bind(){
        glBindTexture(type, ID);
    }

	// Unbinds a texture
	void unbind(){
        glBindTexture(type, 0);
    }

	// Deletes a texture
	void destroy(){
        glDeleteTextures(1, &ID);
    }
};
#endif