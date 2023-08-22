#version 330 core
layout(location = 0) in vec3 aVerts;// 0th attribute
layout(location = 1) in vec2 aTexCoord;// 2nd attribute


out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection*view*model*vec4(aVerts.x, aVerts.y, aVerts.z,1.0);
	TexCoord = aTexCoord;	
}