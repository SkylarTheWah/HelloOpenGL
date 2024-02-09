#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//debug printing
#define DEBUG 1
#define DEBUG_PRINT(args) do { if (DEBUG) std::cout << args << std::endl; } while(0)

#define WIDTH 800
#define HEIGHT 600


void framebuffer_size_callback(GLFWwindow * window, int width, int height);
void processInput(GLFWwindow * window);
void render();

void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
void render()
{
	GLfloat verticies[] =
	{
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	//declare vbo and generate and bind it to a buffer and copy it
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);



}
int main(int argc, char** argv)
{
	//initialise OpenGL
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//now create the window
	GLFWwindow* window = glfwCreateWindow(800, 600, "HelloOpenGL", NULL, NULL);
	if (window == NULL)
	{
		DEBUG_PRINT("Failed to create GLFW Window :(");
		glfwTerminate();
		return -1;
	}
	DEBUG_PRINT("GLFW window successfully created!");

	glfwMakeContextCurrent(window);

	//now initialise GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		DEBUG_PRINT("Failed to initialise GLAD :(");
		return -1;
	}
	DEBUG_PRINT("GLAD initialised!");

	//set viewport dimentions
	glViewport(0, 0, WIDTH, HEIGHT);

	//register callback
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// *******************************************
	// * HERE BE WHERE THE RENDER LOOP BEGINS!!! *
	// *******************************************
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		
		//do rendering
		render();

		//poll events then swap the buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwTerminate();

	return 0;
}