#version 330 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
layout (location = 1) in vec3 aColor; //color variable has attribute position 1

out vec3 ourColor; // specify a color output to the fragment shader
uniform float scale;

void main()
{
    gl_Position = vec4(aPos.x * scale, aPos.y * scale, aPos.z * scale, 1.0);
    ourColor = aColor;
}