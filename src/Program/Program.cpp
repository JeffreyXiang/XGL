#include "Program.h"

namespace XGL
{
	// --- Uniform ---

	template<>
	Uniform<int>& Uniform<int>::operator= (const int& value)
	{
		glUseProgram(program);
		glUniform1i(location, value);
		glUseProgram(0);
		return *this;
	}

	template<>
	Uniform<float>& Uniform<float>::operator= (const float& value)
	{
		glUseProgram(program);
		glUniform1f(location, value);
		glUseProgram(0);
		return *this;
	}

	template<>
	Uniform<Vec2>& Uniform<Vec2>::operator= (const Vec2& value)
	{
		glUseProgram(program);
		glUniform2fv(location, 1, value.getData());
		glUseProgram(0);
		return *this;
	}

	template<>
	Uniform<Vec3>& Uniform<Vec3>::operator= (const Vec3& value)
	{
		glUseProgram(program);
		value == Vec3(0,0,0);
		glUniform3fv(location, 1, value.getData());
		glUseProgram(0);
		return *this;
	}

	template<>
	Uniform<Vec4>& Uniform<Vec4>::operator= (const Vec4& value)
	{
		glUseProgram(program);
		glUniform4fv(location, 1, value.getData());
		glUseProgram(0);
		return *this;
	}

	template<>
	Uniform<Mat2>& Uniform<Mat2>::operator= (const Mat2& value)
	{
		glUseProgram(program);
		glUniformMatrix2fv(location, 1, value.getMajor(), value.getData());
		glUseProgram(0);
		return *this;
	}

	template<>
	Uniform<Mat3>& Uniform<Mat3>::operator= (const Mat3& value)
	{
		glUseProgram(program);
		glUniformMatrix3fv(location, 1, value.getMajor(), value.getData());
		glUseProgram(0);
		return *this;
	}

	template<>
	Uniform<Mat4>& Uniform<Mat4>::operator= (const Mat4& value)
	{
		glUseProgram(program);
		glUniformMatrix4fv(location, 1, value.getMajor(), value.getData());
		glUseProgram(0);
		return *this;
	}

    // --- Program ---

    bool Program::linkOutput()
    {
        int success;
        glGetProgramiv(handle, GL_LINK_STATUS, &success);
        if (!success)
        {
            char* info = new char[2048];
            glGetProgramInfoLog(handle, 2048, NULL, info);
            std::cerr << "ERROR | XGL::Program::linkOutput() : Link failed.\n" << info << std::endl;
            delete[] info;
            throw LINK_FAIL;
        }
        return success;
    }

    void Program::link()
    {
        glLinkProgram(handle);
        linkOutput();
    }

    void Program::use()
    {
        glUseProgram(handle);
    }

	void Program::setCamera(Camera& camera)
	{
		uniform<Mat4>("view") = camera.viewMat();
		uniform<Mat4>("projection") = camera.projectionMat();
	}

	void Program::draw(Object& object)
	{
		uniform<Mat4>("model") = object.modelMat();
		Buffer* buffer = object.genBuffer();
		use();
		buffer->bind();
		glDrawElements(GL_TRIANGLES, object.getVertexNum(), GL_UNSIGNED_INT, NULL);
		glBindVertexArray(0);
		glUseProgram(0);
		delete buffer;
	}
}
