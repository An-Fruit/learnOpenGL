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

    //normalize values to their absolute value so that we don't end up with black
    //colors for everything
    float absX = xPos < 0 ? -1 * xPos : xPos;
    float absY = yPos < 0 ? -1 * yPos : yPos;
    float absZ = zPos < 0 ? -1 * zPos : zPos;

    gl_Position = vec4(xPos, yPos, zPos, 1.0); // see how we directly give a vec3 to vec4's constructor
    ourColor = vec3(absX, absY, absZ);
}