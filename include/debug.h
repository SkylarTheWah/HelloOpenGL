#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//debug logging
#define DEBUG_LOG(args) do { std::cout << args << std::endl; } while(0);

//checking OGL errors
#define glCheckError() _glCheckError(__FILE__, __LINE__)
GLenum _glCheckError(const char* file, int line);

#endif //DEBUG_H
