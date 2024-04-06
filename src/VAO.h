#ifndef VAO_CLASS
#define VAO_CLASS

#include <glad/glad.h>
#include "VBO.h"

class VertArrObj{
    public:
    
    unsigned int ID;

    /**
     * Constructor for Vertex Array Object
     * generates an openGL VAO and binds ID to it
    */
    VertArrObj(){
        glGenVertexArrays(1, &ID);

    }

    /**
     * sets up a vertex attribute
     * @param index specifies the index of the generic vertex attribute to be modified.
     * @param stride specifies the byte offset between consecutive generic vertex attributes. 
     * @param init_offset specifies a offset of the first component of the first generic vertex attribute in the array 
     *                    in the data store of the buffer currently bound to the GL_ARRAY_BUFFER target.
    */
    void linkAttrib(VertBufObj &VBO, unsigned int layout, unsigned int numComponents, GLenum type, unsigned int stride, void* offset){
        VBO.bind();
        glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
        glEnableVertexAttribArray(layout);
        VBO.unbind();
    }

    /**
     * pre: none
     * post: binds the VAO specified by ID
    */
    void bind(){
        glBindVertexArray(ID);

    }

    /**
     * pre: none
     * post: unbinds the currently active VAO
    */
    void unbind(){
        glBindVertexArray(0);
    }

    /**
     * pre: none
     * post: deletes the VAO associated with ID
    */
    void destroy(){
        glDeleteVertexArrays(1, &ID);
    }

};

#endif