#include "debug.h"

GLenum _glCheckError(const char* file, int line)
{
	GLenum code;
	while ((code = glGetError()) != GL_NO_ERROR)
	{
		std::string err;
		switch (code)
		{
			case GL_INVALID_ENUM: err = "GL_INVALID_ENUM"; break;
			case GL_INVALID_VALUE: err = "GL_INVALID_VALUE"; break;
			case GL_INVALID_OPERATION: err = "GL_INVALID_OPERATION"; break;
			case GL_STACK_OVERFLOW: err = "GL_STACK_OVERFLOW"; break;
			case GL_STACK_UNDERFLOW: err = "GL_STACK_UNDERFLOW"; break;
			case GL_OUT_OF_MEMORY: err = "GL_OUT_OF_MEMORY"; break;
			case GL_INVALID_FRAMEBUFFER_OPERATION: err = "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		DEBUG_LOG(err << " | " << file << " (" << line << ")");
		return code;
	}
}
