//stdlib incs
#include <iostream>
#include <cmath>

//gl incs
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"

#ifndef DEBUG
#define DEBUG 1
#include "debug.h"
#endif



//forward declarations

void framebuffer_size_callback(GLFWwindow * window, int width, int height);
void processInput(GLFWwindow * window);
void createBuffers();
void destroyBuffers();
void createShaders();
void render();

//our window handle
GLFWwindow* window;

//the verticies for the triangle to be drawn
float verticies[] = {
	//positions			//colours
	 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, //bottom right
	-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, //bottom left
	 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f //top
};

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"		//attrib pos 0
"layout (location = 1) in vec3 aColor;\n"	//attrib pos 1
"out vec3 ourColor;\n" //colour to send to frag shader
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos, 1.0f);\n"
"	ourColor = aColor;\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(ourColor, 1.0f);\n"
"}\0";

unsigned int vertexShader;
unsigned int fragmentShader;
unsigned int shaderProgram;
unsigned int VBO;
unsigned int VAO;

void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

bool initialiseGL(const int height, const int width)
{
	//initialise OpenGL
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//debug output for now
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

	//now create the window
	window = glfwCreateWindow(height, width, "HelloOpenGL", NULL, NULL);
	if (window == NULL)
	{
		DEBUG_LOG("Failed to create GLFW Window :(");
		glfwTerminate();
		return false;
	}
	DEBUG_LOG("GLFW window successfully created!");

	glfwMakeContextCurrent(window);

	//now initialise GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		DEBUG_LOG("Failed to initialise GLAD :(");
		return false;
	}
	DEBUG_LOG("GLAD initialised!");
	return true;
}

void createBuffers()
{
	//generate buffs
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	//bind the vao
	glBindVertexArray(VAO);

	//copy triangle verts into the vbo
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

	//set pos attrib pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//set colour attrib pointers
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void destroyBuffers()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
}

void createShaders()
{
	//VERTEX SHADER
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		DEBUG_LOG("ERROR!! Unable to compile vertex shader!!" << infoLog);
	}

	//FRAGMENT SHADER
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		DEBUG_LOG("ERROR!! Unable to compile fragment shader" << infoLog);	
	}
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		DEBUG_LOG("ERROR!! Unable to link shader program" << infoLog);	
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void render()
{
	//set background colour
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//draw the tris
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main()
{
	const int height = 800;
	const int width = 600;

	if (!initialiseGL(height, width))
	{
		DEBUG_LOG("Program will now terminate");
		return -1;
	}

	//set viewport dimentions
	glViewport(0, 0, height, width);

	//register callback
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//int nrAttribs;
	//glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttribs);
	//std::cout << "max nr of vert attribs: " << nrAttribs << std::endl;

	//now create the shaders
	createShaders();

	//now create buffs
	createBuffers();

	//access shader
	glUseProgram(shaderProgram);

	// *******************************************
	// * HERE BE WHERE THE RENDER LOOP BEGINS!!! *
	// *******************************************
	while (!glfwWindowShouldClose(window))
	{
		//process any user input - monitoring for esc to close app
		processInput(window);

		//do rendering
		render();

		//swap the buffers the poll IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	destroyBuffers();
	glfwTerminate();

	return 0;
}
