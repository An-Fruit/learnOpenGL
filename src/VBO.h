#ifndef VBO_CLASS
#define VBO_CLASS

#include <glad/glad.h>

class VertBufObj{
    public:
        unsigned int ID;
        /**
         * constructs new vertex buffer object using a float matrix of defined size
         * @param vertices pointer to the beginning of the float matrix
         * @param size the size of the matrix
         * pre: vertices is a valid matrix of size bytes
         * post: an openGL VBO is initialized to the values of the float matrix
         *      and a reference is assigned to the global var ID. 
        */
        VertBufObj(float *vertices, size_t size, GLenum usage){
            glGenBuffers(1, &ID);
            glBindBuffer(GL_ARRAY_BUFFER, ID);
            glBufferData(GL_ARRAY_BUFFER, size, vertices, usage);
        }
        /**
         * pre: none
         * post: binds the VBO referenced by ID 
        */
        void bind(){
            glBindBuffer(GL_ARRAY_BUFFER, ID);
        }

        /**
         * pre: none
         * post: unbinds GL_ARRAY_BUFFER
         * **WARNING** it does not matter if this VBO object is not currently active, calling unbind will still
         *             unbind the array buffer
        */
        void unbind(){
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        /**
         * pre: none
         * post: deletes the buffer referenced by ID
        */
        void destroy(){
            glDeleteBuffers(1, &ID);
        }

};
#endif