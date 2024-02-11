#include "shader.h"

Shader::Shader(const char* vertexPath, const char* fragPath)
{
	//retrive sources of shaders
	std::string vertCode;
	std::string fragCode;
	std::ifstream vshFile;
	std::ifstream fshFile;

	//ensure throwing exceptions
	vshFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fshFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		//open the files
		vshFile.open(vertexPath);
		fshFile.open(fragPath);
		std::stringstream vshStream, fshStream;
		//read files into streams
		vshStream << vshFile.rdbuf();
		fshStream << fshFile.rdbuf();
		//close handlers
		vshFile.close();
		fshFile.close();
		//convert into strings
		vertCode = vshStream.str();
		fragCode = fshStream.str();
	}
	catch (std::ifstream::failure e)
	{
		DEBUG_LOG("ERROR: Shader file not successfully read");
		DEBUG_LOG(e.what());
	}
	//save parsed shader code into temp bufs
	const char* vshCode = vertCode.c_str();
	const char* fshCode = fragCode.c_str();

	//compilation boilerplate wheeeeeeeeeeeeeeeee :)
	GLuint vert, frag;
	GLint success;
	char infoLog[512];

	//vert shader
	vert = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert, 1, &vshCode, NULL);
	glCompileShader(vert);
	glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vert, 512, NULL, infoLog);
		DEBUG_LOG("ERROR: Vertex shader compilation failed...\n" << infoLog);
	}

	//frag
	frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag, 1, &fshCode, NULL);
	glCompileShader(frag);
	glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(frag, 512, NULL, infoLog);
		DEBUG_LOG("ERROR: Fragment shader compilation failed...\n" << infoLog);
	}

	//link to shader binary for submission to gpu
	ID = glCreateProgram();
	glAttachShader(ID, vert);
	glAttachShader(ID, frag);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		DEBUG_LOG("ERROR: Fragment shader compilation failed...\n" << infoLog);
	}

	//delete shaders
	glDeleteShader(vert);
	glDeleteShader(frag);
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool val) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)val);
}

void Shader::setInt(const std::string& name, int val) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), val);
}

void Shader::setFloat(const std::string& name, float val) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), val);
}
