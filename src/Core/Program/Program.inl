#ifndef XGL_PROGRAM_INL
#define XGL_PROGRAM_INL

#include "Program.h"

#include <fstream>
#include <sstream>

namespace XGL
{
	// --- Shader ---

	template<ShaderType type>
	bool Shader<type>::compileOutput()
	{
		int success;
		glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char* info = new char[2048];
			glGetProgramInfoLog(handle, 2048, NULL, info);
			std::cerr << "ERROR | XGL::Shader::compileOutput() : Compilation failed.\n" << info << std::endl;
			delete[] info;
			throw COMPILE_FAIL;
		}
		return success;
	}

	template<ShaderType type>
	Shader<type>::Shader()
	{
		code = NULL;
		if (type == VERTEX)
			handle = glCreateShader(GL_VERTEX_SHADER);
		else if (type == FRAGMENT)
			handle = glCreateShader(GL_FRAGMENT_SHADER);
		else
		{
			std::cerr << "ERROR | XGL::Shader::Shader() : No such shader type.\n";
			throw NO_SUCH_TYPE;
		}
	}

	template<ShaderType type>
	void Shader<type>::load(const char* filename)
	{
		if (code)
			delete[] code;
		std::ifstream f;
		std::stringstream buf;
		f.open(filename);
		if (!f.is_open())
		{
			std::cerr << "ERROR | XGL::Shader::load(const char*) : Failed to open file \"" << filename << "\".\n";
			throw FILE_OPEN_FAIL;
		}
		buf << f.rdbuf();
		f.close();
		int length = buf.str().size();
		code = new char[length + 1];
		strcpy(code, buf.str().c_str());
		glShaderSource(handle, 1, &code, NULL);
	}

	template<ShaderType type>
	void Shader<type>::compile()
	{
		glCompileShader(handle);
		compileOutput();
	}


	// --- Program ---

	template<ShaderType type>
	void Program::attachShader(Shader<type>& shader)
	{
		glAttachShader(handle, shader.getHandle());
	}

	template<typename T>
	Uniform<T> Program::uniform(const char* name)
	{
		int location = glGetUniformLocation(handle, name);
		if (location == -1)
		{
			std::cerr << "ERROR | XGL::Program::uniform(const char*) : No such uniform.\n";
			throw NO_SUCH_UNIFORM;
		}
		return Uniform<T>(handle, location);
	}
}

#endif // !XGL_PROGRAM_INL
