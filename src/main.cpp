//stdlib incs
#include <iostream>
#include <cmath>

//gl incs
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//#include "shader.h"

#define DEBUG 1

#include "debug.h"
#include "shader.h"
//forward declarations
void framebuffer_size_callback(GLFWwindow * window, int width, int height);
void processInput(GLFWwindow * window);
void createBuffers();
void destroyBuffers();
void printAdapterInfo();

//our window handle
GLFWwindow* window;

// first triangle
float verticies[] = {
	// positions         // colors
	 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
	 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
};

GLuint vao;
GLuint vbo;

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
	DEBUG_LOG("*****BEGIN INIT GL*****");
	//initialise OpenGL
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
	DEBUG_LOG("*****END INIT GL*****");
	return true;
}

void createBuffers()
{
	//generate buffs
	glGenBuffers(1, &vao);
	glGenVertexArrays(1, &vbo);

	//now bind the VAO to use it
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

	//tri vert's
	glCheckError(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0));
	glEnableVertexAttribArray(0);
	//tri color;s
	glCheckError(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))));
	glEnableVertexAttribArray(1);
}

void destroyBuffers()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

void printAdapterInfo()
{
	DEBUG_LOG("----------------------------------------------------------");
	DEBUG_LOG("Adapter info:")
	DEBUG_LOG("GL_VENDOR: " << glGetString(GL_VENDOR));
	DEBUG_LOG("GL_RENDERER: " << glGetString(GL_RENDERER));
	DEBUG_LOG("GL_VERSION: " << glGetString(GL_VERSION));
	DEBUG_LOG("GL_SHADING_LANGUAGE_VERSION: " << glGetString(GL_SHADING_LANGUAGE_VERSION));
	DEBUG_LOG("----------------------------------------------------------");
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
	Shader shaders = Shader::Shader("../shaders/vert.glsl", "../shaders/frag.glsl");
	
	//now create buffs
	createBuffers();

	// *******************************************
	// * HERE BE WHERE THE RENDER LOOP BEGINS!!! *
	// *******************************************
	while (!glfwWindowShouldClose(window))
	{
		//process any user input - monitoring for esc to close app
		processInput(window);

		//clear color of screen
		glClearColor(0.5f, 0.3f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//now render our triangle
		shaders.use();
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//swap the buffers the poll IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	destroyBuffers();
	glfwTerminate();

	return 0;
}
