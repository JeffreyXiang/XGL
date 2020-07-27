#ifndef XGL_PROGRAM_H
#define XGL_PROGRAM_H

#include <glad/glad.h>
#include <Math/Vector.h>
#include <Math/Matrix.h>
#include "Object/Object.h"
#include "Camera/Camera.h"

namespace XGL
{
	enum ShaderType { VERTEX, FRAGMENT };

	template<ShaderType type>
	class Shader
	{
		public:
			enum ERROR { NO_SUCH_TYPE, FILE_OPEN_FAIL, COMPILE_FAIL };

		private:
			unsigned int handle;
			char* code;

			bool compileOutput();

		public:
			Shader();
			Shader(const char* filename) : Shader() { load(filename); }
			~Shader() { delete[] code; glDeleteShader(handle); }

			void load(const char* filename);
			void compile();

			unsigned int getHandle() { return handle; }
			unsigned int getCode() { return code; }
	};

	template<typename T>
	class Uniform
	{
		public:
			enum ERROR { NO_SUCH_UNIFORM, LINK_FAIL };

		private:
			unsigned int program;
			int location;

		public:
			Uniform(int program, int location) : program(program), location(location) {}
			Uniform(const Uniform<T>& other) : program(other.program), location(other.location) {}
			~Uniform() {}

			Uniform<T>& operator= (const T& value);
			Uniform<T>& operator= (T&& value) { (*this) = value; return *this; }
	};

	template<>
	Uniform<int>& Uniform<int>::operator= (const int& value);
	template<>
	Uniform<float>& Uniform<float>::operator= (const float& value);
	template<>
	Uniform<Vec2>& Uniform<Vec2>::operator= (const Vec2& value);
	template<>
	Uniform<Vec3>& Uniform<Vec3>::operator= (const Vec3& value);
	template<>
	Uniform<Vec4>& Uniform<Vec4>::operator= (const Vec4& value);
	template<>
	Uniform<Mat2>& Uniform<Mat2>::operator= (const Mat2& value);
	template<>
	Uniform<Mat3>& Uniform<Mat3>::operator= (const Mat3& value);
	template<>
	Uniform<Mat4>& Uniform<Mat4>::operator= (const Mat4& value);

	class Program
	{
		public:
			enum ERROR { NO_SUCH_UNIFORM, LINK_FAIL, NO_CAMERA };

		private:
			unsigned int handle;
			Camera* camera;

			bool linkOutput();

		public:
			Program() : handle(glCreateProgram()), camera(NULL) {}
			~Program() { glDeleteProgram(handle); }

			void setCamera(Camera& camera) { this->camera = &camera; }
			void updateCamera(float deltaT);
			template<ShaderType type>
			void attachShader(Shader<type>& shader);
			void link();
			void use();
			void draw(Object& object);

			unsigned int getHandle() { return handle; }

			template<typename T>
			Uniform<T> uniform(const char* name);
	};
}

#include "Program.inl"

#endif // !XGL_PROGRAM_H
