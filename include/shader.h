#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> //need ogl headers

#include "debug.h" //DEBUG_LOG

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	//program id
	unsigned int ID;

	//constructer
	Shader(const char* vertPath, const char* fragPath);

	//use/activate shader
	void use();
	//utility funcs
	void setBool(const std::string& name, bool val) const;
	void setInt(const std::string& name, int val) const;
	void setFloat(const std::string& name, float val) const;
};

#endif
