#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//debug logging
#define DEBUG_LOG(args) do { std::cerr << args << std::endl; } while(0);

//checking OGL errors
class Debug
{
public:
	static void _glCheckError(const char* file, int line, const char* stmt);
};

#ifdef DEBUG
#define glCheckError(stmt) do { stmt; Debug::_glCheckError(__FILE__, __LINE__, #stmt); } while(0)
#else
#define glCheckError(stmt) stmt
#endif

#endif //DEBUG_H
