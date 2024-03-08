#version 330 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
layout (location = 1) in vec3 aColor; //color variable has attribute position 1

out vec3 ourColor; // specify a color output to the fragment shader
uniform float horizOffset;

void main()
{
    float xPos = aPos.x;
    float yPos = aPos.y;
    float zPos = aPos.z;

    //absolute values for position
    //use this for colors

    gl_Position = vec4(xPos, yPos, zPos, 1.0); // see how we directly give a vec3 to vec4's constructor
    ourColor = aColor;
}