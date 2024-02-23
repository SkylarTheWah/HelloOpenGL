//stdlib incs
#include <iostream>
#include <cmath>

//gl incs
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//#include "shader.h"

#define DEBUG 1

#include "debug.h"
//forward declarations
void framebuffer_size_callback(GLFWwindow * window, int width, int height);
void processInput(GLFWwindow * window);
void createBuffers();
void destroyBuffers();
void createShaders();
void render();
void printAdapterInfo();

//our window handle
GLFWwindow* window;

// first triangle
float tri1[] = {
	-0.9f, -0.5f, 0.0f,  // left 
	-0.0f, -0.5f, 0.0f,  // right
	-0.45f, 0.5f, 0.0f  // top 
};

// second triangle
float tri2[] = {
	0.0f, -0.5f, 0.0f,  // left
	0.9f, -0.5f, 0.0f,  // right
	0.45f, 0.5f, 0.0f   // top 
};

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos, 1.0f);\n"
"}\0";

const char* pinkFragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);\n"
"}\0";

const char* yellowFragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"}\0";

unsigned int vertexShader;
unsigned int pinkFragmentShader;
unsigned int yellowFragmentShader;
unsigned int pinkShaderProgram;
unsigned int yellowShaderProgram;
GLuint vao[2];
GLuint vbo[2];


void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

static bool initialiseGL(const int height, const int width)
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
		DEBUG_LOG("ERROR!! Failed to create GLFW Window :(");
		glfwTerminate();
		return false;
	}
	DEBUG_LOG("GLFW window successfully created!");

	glfwMakeContextCurrent(window);

	//now initialise GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		DEBUG_LOG("ERROR!! Failed to initialise GLAD :(");
		return false;
	}
	DEBUG_LOG("GLAD initialised!");
	return true;
}

void createBuffers()
{
	//generate buffs
	glGenBuffers(2, vao);
	glGenVertexArrays(2, vbo);

	//tri1
	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tri1), tri1, GL_STATIC_DRAW);
	glCheckError(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
	glEnableVertexAttribArray(0);

	//tri2
	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tri2), tri2, GL_STATIC_DRAW);
	glCheckError(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
	glEnableVertexAttribArray(0);

	//glCheckError();
}

void destroyBuffers()
{
	glDeleteVertexArrays(2, vao);
	glDeleteBuffers(2, vbo);
	glDeleteProgram(pinkShaderProgram);
	glDeleteProgram(yellowShaderProgram);
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

	//PINK FRAGMENT SHADER
	pinkFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(pinkFragmentShader, 1, &pinkFragmentShaderSource, NULL);
	glCompileShader(pinkFragmentShader);

	glGetShaderiv(pinkFragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(pinkFragmentShader, 512, NULL, infoLog);
		DEBUG_LOG("ERROR!! Unable to compile fragment shader" << infoLog);	
	}
	//PINK SHADER PROG
	pinkShaderProgram = glCreateProgram();

	glAttachShader(pinkShaderProgram, vertexShader);
	glAttachShader(pinkShaderProgram, pinkFragmentShader);
	glLinkProgram(pinkShaderProgram);

	glGetProgramiv(pinkShaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(pinkShaderProgram, 512, NULL, infoLog);
		DEBUG_LOG("ERROR!! Unable to link shader program" << infoLog);
	}
	glDeleteShader(pinkFragmentShader);
	
	//YELLOWFRAGMENT SHADER
	yellowFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(yellowFragmentShader, 1, &yellowFragmentShaderSource, NULL);
	glCompileShader(yellowFragmentShader);

	glGetShaderiv(yellowFragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(yellowFragmentShader, 512, NULL, infoLog);
		DEBUG_LOG("ERROR!! Unable to compile fragment shader" << infoLog);	
	}
	yellowShaderProgram = glCreateProgram();

	glAttachShader(yellowShaderProgram, vertexShader);
	glAttachShader(yellowShaderProgram, yellowFragmentShader);
	glLinkProgram(yellowShaderProgram);

	glGetProgramiv(yellowShaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(yellowShaderProgram, 512, NULL, infoLog);
		DEBUG_LOG("ERROR!! Unable to link shader program" << infoLog);	
	}
	glDeleteShader(vertexShader);
	glDeleteShader(yellowFragmentShader);
}

void render()
{
	//set background colour
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//draw tri1
	glUseProgram(pinkShaderProgram);
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	//draw tri2
	glUseProgram(yellowShaderProgram);
	glBindVertexArray(vao[1]);
	glDrawArrays(GL_TRIANGLES, 0, 3);

}

void printAdapterInfo()
{
	const GLubyte* glVendor = glGetString(GL_VENDOR);
	const GLubyte* glRenderer =  glGetString(GL_RENDERER);
	const GLubyte* glVersion = glGetString(GL_VERSION);
	const GLubyte* glSLVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	DEBUG_LOG("GL Vendor name: " << glVendor);
	DEBUG_LOG("GL Adapter name: " << glRenderer);
	DEBUG_LOG("GL Version: " << glVersion);
	DEBUG_LOG("GLSL version: " << glSLVersion);
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
	DEBUG_LOG("Viewport initialised with height: " << height << " and width: " << width);

	printAdapterInfo();
	//register callback
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//now create the shaders
	createShaders();

	//now create buffs
	createBuffers();

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
