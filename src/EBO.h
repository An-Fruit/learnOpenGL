#ifndef EBO_CLASS
#define EBO_CLASS

#include <glad/glad.h>

class ElemBufObj{
    public:
        unsigned int ID;
        /**
         * constructs new vertex buffer object using an int array of defined size
         * @param vertices pointer to the beginning of the int array
         * @param size the size of the array
         * pre: drawOrder is a valid int array of size bytes
         * post: an openGL EBO is initialized to the values of the int array
         *      and a reference is assigned to the global var ID. 
        */
        ElemBufObj(int *drawOrder, size_t size, GLenum usage){
            glGenBuffers(1, &ID);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, drawOrder, usage);
        }
        /**
         * pre: none
         * post: binds the EBO referenced by ID 
        */
        void bind(){
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
        }

        /**
         * pre: none
         * post: unbinds GL_ARRAY_BUFFER
         * **WARNING** it does not matter if this EBO object is not currently active, calling unbind will still
         *             unbind the array buffer
        */
        void unbind(){
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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