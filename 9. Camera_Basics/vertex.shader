#version 330 core
layout (location = 0) in vec3 aPos;//1st attribute
layout(location = 1) in vec2 aUVCoords;//2nd attribute

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos.xyz,1.0);
	TexCoords = aUVCoords;
	//To do operations on each vertex
}