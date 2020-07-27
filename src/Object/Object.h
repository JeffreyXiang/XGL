#ifndef XGL_OBJECT_H
#define XGL_OBJECT_H

#include <Math/Vector.h>
#include <Math/Matrix.h>
#include <Math/Transform.h>
#include <Texture.h>
#include <glad/glad.h>

#include <vector>

namespace XGL
{
	class Buffer
	{
		public:
			enum ERROR{ INDEX_COLLISION, NO_SUCH_TYPE, INDICES_EXISTED };
			enum Type { BYTE, UNSIGNED_BYTE, SHORT, UNSIGNED_SHORT, INT, UNSIGNED_INT, FLOAT };
			enum Usage { STREAM, STATIC, DYNAMIC };

			typedef struct
			{
				unsigned int index;
				int size;
				Type type;
				bool normalized;
			} Format;

			typedef struct
			{
				unsigned int index;
				int size;
				GLenum type;
				bool normalized;
				size_t structSize;
				size_t offset;
			} FormatDetail;

		private:
			// --- gl data ---
			unsigned int VAOHandle;
			std::vector<unsigned int> VBOHandle;
			unsigned int EBOHandle;

			std::vector<FormatDetail> format;

			bool addFormat(FormatDetail format);

			size_t getTypeSize(Type type);
			GLenum getTypeGL(Type type);

		public:
			Buffer() : EBOHandle(0) { glGenVertexArrays(1, &VAOHandle); }
			~Buffer();

			void addData(void* data, size_t size, Usage usage, std::vector<Format> format);
			void addData(void* data, size_t size, Usage usage, std::vector<FormatDetail> format);
			void addIndex(unsigned int* indices, size_t size, Usage usage);

			void bind() { glBindVertexArray(VAOHandle); }

			unsigned int getHandle() { return VAOHandle; }
	};

	class Object
	{
		public:
			enum ERROR { MODEL_DATA_MISMATCH };

			typedef struct
			{
				Texture* texture;
				const char* name;
				unsigned int unit;
			} textureInfo;

		private:
			// --- model data ---
			struct
			{
				std::vector<Vec3> positions;
				std::vector<Vec3> normals;
				std::vector<Vec2> texcoords;
				std::vector<unsigned int> indices;
			} modelData;

			// --- texture ---
			std::vector<textureInfo> textures;

			// --- world data ---
			Vec3 position;
			Mat4 rotateMat;
			float scaleX, scaleY, scaleZ;
			Mat4 model;

		public:
			Object() :
				rotateMat(Mat4::identity()),
				scaleX(1), scaleY(1), scaleZ(1) {}
			~Object() {}

			void setModelPositions(std::vector<Vec3>& positions) { modelData.positions = positions; }
			void setModelNormals(std::vector<Vec3>& normals) { modelData.normals = normals; }
			void setModelTexcoords(std::vector<Vec2>& texcoords) { modelData.texcoords = texcoords; }
			void setModelIndices(std::vector<unsigned int> indices) { modelData.indices = indices; }

			void addModelPosition(Vec3 position) { modelData.positions.push_back(position); }
			void addModelNormal(Vec3 normal) { modelData.normals.push_back(normal); }
			void addModelTexcoord(Vec2 texcoord) { modelData.texcoords.push_back(texcoord); }
			void addModelIndex(unsigned int index) { modelData.indices.push_back(index); }

			void setPosition(Vec3 position) { this->position = position; }
			void setRotation(float angle, Vec3 axis) { rotateMat = Transform::rotate(angle, axis); }
			void setScaling(float x, float y, float z) { scaleX = x; scaleY = y; scaleZ = z; }
			void setScaling(float f) { scaleX = f; scaleY = f; scaleZ = f; }
			Mat4& modelMat();

			void addTexture(Texture& tex, const char* name, unsigned int unit);
			void addTexture(Texture& tex, const char* name);

			size_t getVertexNum() { return modelData.indices.size(); }
			std::vector<textureInfo>& getTextures() { return textures; }

			Buffer* genBuffer();
	};
}

#endif // !XGL_OBJECT_H
