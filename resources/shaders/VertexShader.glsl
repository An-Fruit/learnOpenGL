// #version 330 core
// layout (location = 0) in vec3 aPos;     // the position variable has attribute position 0
// layout (location = 1) in vec3 aColor;   //color variable has attribute position 1
// layout (location = 2) in vec2 aText;    //texture ocordinates

// out vec3 ourColor;  // specify a color output to the fragment shader
// out vec2 texCoord; // specify texture coordinates for a vertex

// uniform float scale;

// void main()
// {
//     gl_Position = vec4(aPos.x * scale, aPos.y * scale, aPos.z * scale, 1.0);
//     ourColor = aColor;
//     texCoord = aText;
// }

#version 330 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Colors
layout (location = 1) in vec3 aColor;
// Texture Coordinates
layout (location = 2) in vec2 aTex;


// Outputs the color for the Fragment Shader
out vec3 color;
// Outputs the texture coordinates to the fragment shader
out vec2 texCoord;

// Controls the scale of the vertices
uniform float scale;


void main()
{
	// Outputs the positions/coordinates of all vertices
	gl_Position = vec4(aPos.x + aPos.x * scale, aPos.y + aPos.y * scale, aPos.z + aPos.z * scale, 1.0);
	// Assigns the colors from the Vertex Data to "color"
	color = aColor;
	// Assigns the texture coordinates from the Vertex Data to "texCoord"
	texCoord = aTex;
}