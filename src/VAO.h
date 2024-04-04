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
     * links a vertex buffer object to the vertex array object
     * @param index specifies the index of the generic vertex attribute to be modified.
     * @param stride specifies the byte offset between consecutive generic vertex attributes. 
     * @param init_offset specifies a offset of the first component of the first generic vertex attribute in the array 
     *                    in the data store of the buffer currently bound to the GL_ARRAY_BUFFER target.
    */
    void linkVBO(VertBufObj VBO, unsigned int index, unsigned int stride, unsigned int init_offset){
        VBO.bind();
        glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, stride, (void*)init_offset);
        glEnableVertexAttribArray(index);
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