#version 330 core

layout (location = 0) in vec3 pos;		//attrib pos 0
layout (location = 1) in vec3 color;	//attrib pos 1

out vec3 ourColor;

void main()
{
	gl_Position = vec4(pos, 1.0f);
	ourColor = color;
}
