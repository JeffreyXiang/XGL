#include "Object.h"

namespace XGL
{
	Buffer::~Buffer()
	{
		for (size_t i = 0; i < VBOHandle.size(); i++)
			glDeleteBuffers(1, &VBOHandle[i]);
		glDeleteBuffers(1, &EBOHandle);
		glDeleteVertexArrays(1, &VAOHandle);
	}

	bool Buffer::addFormat(FormatDetail format)
	{
		auto itr = this->format.begin();
		for (; itr < this->format.end(); itr++)
		{
			if (format.index == itr->index)
				return false;
			else if (format.index < itr->index)
				break;
		}
		this->format.insert(itr, format);
		return true;
	}

	size_t Buffer::getTypeSize(Type type)
	{
		switch (type)
		{
			case BYTE:
			case UNSIGNED_BYTE:
				return sizeof(char);
			case SHORT:
			case UNSIGNED_SHORT:
				return sizeof(short);
			case INT:
			case UNSIGNED_INT:
				return sizeof(int);
			case FLOAT:
				return sizeof(float);
		}
		return -1;
	}

	GLenum Buffer::getTypeGL(Type type)
	{
		switch (type)
		{
			case BYTE:
				return GL_BYTE;
			case UNSIGNED_BYTE:
				return GL_UNSIGNED_BYTE;
			case SHORT:
				return GL_SHORT;
			case UNSIGNED_SHORT:
				return GL_UNSIGNED_SHORT;
			case INT:
				return GL_INT;
			case UNSIGNED_INT:
				return GL_UNSIGNED_INT;
			case FLOAT:
				return GL_FLOAT;
		}
		return GL_FALSE;
	}

	void Buffer::addData(void* data, size_t size, Usage usage, std::vector<Format> format)
	{
		unsigned int VBO;
		glGenBuffers(1, &VBO);
		VBOHandle.push_back(VBO);

		// --- calculate detail ---

		size_t* elemOffset = new size_t[format.size() + 1];
		elemOffset[0] = 0;
		size_t typeSize;

		for (size_t i = 0; i < format.size(); i++)
		{
			typeSize = getTypeSize(format[i].type);
			if (typeSize == -1)
			{
				std::cerr << "ERROR | XGL::Buffer::addData(void*, size_t, Usage, std::vector<Format>) : No such type.\n";
				throw NO_SUCH_TYPE;
			}
			elemOffset[i + 1] = elemOffset[i] + format[i].size * typeSize;
		}

		for (size_t i = 0; i < format.size(); i++)
		{
			if (!addFormat({
				format[i].index,
				format[i].size,
				getTypeGL(format[i].type),
				format[i].normalized,
				elemOffset[format.size()],
				elemOffset[i] }))
			{
				std::cerr << "ERROR | XGL::Buffer::addData(void*, size_t, Usage, std::vector<Format>) : Index collision.\n";
				throw INDEX_COLLISION;
			}
		}

		glBindVertexArray(VAOHandle);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		switch (usage)
		{
			case STREAM:
				glBufferData(GL_ARRAY_BUFFER, size, data, GL_STREAM_DRAW); break;
			case STATIC:
				glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW); break;
			case DYNAMIC:
				glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW); break;
		}

		for (size_t i = 0; i < format.size(); i++)
		{
			glVertexAttribPointer(
				format[i].index, format[i].size, getTypeGL(format[i].type),
				format[i].normalized, elemOffset[format.size()], (void*)elemOffset[i]);
			glEnableVertexAttribArray(format[i].index);
		}

		glBindVertexArray(0);

		delete[] elemOffset;
	}

	void Buffer::addData(void* data, size_t size, Usage usage, std::vector<FormatDetail> format)
	{
		unsigned int VBO;
		glGenBuffers(1, &VBO);
		VBOHandle.push_back(VBO);

		for (size_t i = 0; i < format.size(); i++)
		{
			if (!addFormat(format[i]))
			{
				std::cerr << "ERROR | XGL::Buffer::addData(void*, size_t, Usage, std::vector<FormatDetail>) : Index collision.\n";
				throw INDEX_COLLISION;
			}
		}

		glBindVertexArray(VAOHandle);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		switch (usage)
		{
			case STREAM:
				glBufferData(GL_ARRAY_BUFFER, size, data, GL_STREAM_DRAW); break;
			case STATIC:
				glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW); break;
			case DYNAMIC:
				glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW); break;
		}

		for (size_t i = 0; i < format.size(); i++)
		{
			glVertexAttribPointer(
				format[i].index, format[i].size, format[i].type,
				format[i].normalized, format[i].structSize, (void*)format[i].offset);
			glEnableVertexAttribArray(format[i].index);
		}

		glBindVertexArray(0);
	}

	void Buffer::addIndex(unsigned int* indices, size_t size, Usage usage)
	{
		if (EBOHandle)
		{
			std::cerr << "ERROR | XGL::Buffer::addIndex(unsigned int*, size_t) : Indices(EBO) already existed.\n";
			throw INDICES_EXISTED;
		}

		glBindVertexArray(VAOHandle);
		glGenBuffers(1, &EBOHandle);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOHandle);

		switch (usage)
		{
			case STREAM:
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STREAM_DRAW); break;
			case STATIC:
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW); break;
			case DYNAMIC:
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_DYNAMIC_DRAW); break;
		}

		glBindVertexArray(0);
	}

	Mat4& Object::modelMat()
	{
		model = rotateMat;
		Transform::scale(model, scaleX, scaleY, scaleZ);
		Transform::translate(model, position);
		return model;
	}

	void Object::addTexture(Texture& tex, const char* name, unsigned int unit)
	{ 
		if (!tex.isGenerated())
			tex.generate();
		textures.push_back({ &tex, name, unit });
	}

	void Object::addTexture(Texture& tex, const char* name)
	{
		unsigned int unit;

		if (textures.size() == 0)
			unit = 0;
		else
		{
			unsigned int* list = new unsigned int[textures.size()];
			for (size_t i = 0; i < textures.size(); i++)
				list[i] = textures[i].unit;

			size_t i = textures.size(), t, temp;

			while ((--i) >= 0) {
				while (0 <= list[i] && list[i] < textures.size() && i != list[i]) {
					t = list[i];
					if (list[i] == list[t]) break;
					temp = list[i];
					list[i] = list[t];
					list[t] = temp;
				}
			}

			i = 0;
			for (; i < textures.size(); ++i)
				if (list[i] != i) break;
			unit = i;
		}
		addTexture(tex, name, unit);
	}

	Buffer* Object::genBuffer()
	{
		if ((modelData.normals.size() && modelData.normals.size() != modelData.positions.size()) ||
			(modelData.texcoords.size() && modelData.texcoords.size() != modelData.positions.size()))
		{
			std::cerr << "ERROR | XGL::Object::genBuffer() : Model data mismatch.\n";
			throw MODEL_DATA_MISMATCH;
		}
		
		size_t vertexSize = sizeof(float) *
			(3 * (modelData.positions.size() > 0) +
			3 * (modelData.normals.size() > 0) +
			2 * (modelData.texcoords.size() > 0));
		size_t VBOSize = modelData.positions.size() * vertexSize;

		std::vector<Buffer::FormatDetail> format;
		char* VBOData = new char[VBOSize];
		size_t offset = 0;

		format.push_back({0, 3, GL_FLOAT, false, vertexSize, offset});
		for (size_t i = 0; i < modelData.positions.size(); i++)
			memcpy(VBOData + vertexSize * i + offset, modelData.positions[i].getData(), 3 * sizeof(float));
		offset += 3 * sizeof(float);

		if (modelData.normals.size())
		{
			format.push_back({ 1, 3, GL_FLOAT, true, vertexSize, offset });
			for (size_t i = 0; i < modelData.positions.size(); i++)
				memcpy(VBOData + vertexSize * i + offset, modelData.normals[i].getData(), 3 * sizeof(float));
			offset += 3 * sizeof(float);
		}

		if (modelData.texcoords.size())
		{
			format.push_back({ 2, 2, GL_FLOAT, false, vertexSize, offset });
			for (size_t i = 0; i < modelData.positions.size(); i++)
				memcpy(VBOData + vertexSize * i + offset, modelData.texcoords[i].getData(), 2 * sizeof(float));
		}

		size_t EBOSize = sizeof(unsigned int) * modelData.indices.size();
		unsigned int* EBOData = new unsigned int[modelData.indices.size()];
		for (size_t i = 0; i < modelData.indices.size(); i++)
			EBOData[i] = modelData.indices[i];

		Buffer* res = new Buffer();
		res->addData(VBOData, VBOSize, Buffer::STATIC, format);
		res->addIndex(EBOData, EBOSize, Buffer::STATIC);
		delete[] VBOData;
		delete[] EBOData;
		return res;
	}
}
