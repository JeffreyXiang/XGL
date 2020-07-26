#include "Texture.h"

#include <iostream>

namespace XGL
{
	void Texture::load(const char* filename)
	{
		if (data)
			stbi_image_free(data);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load(filename, &width, &height, &channel, 3);
		if (!data)
		{
			std::cerr << "ERROR | XGL::Texture::load(const char*) : Failed to open file \"" << filename << "\".\n";
			throw FILE_OPEN_FAIL;
		}
	}

	void Texture::setWrappingPolicy(WrappingPolicy x, WrappingPolicy y)
	{
		wrappingX = x;
		wrappingY = y;
	}

	void Texture::setWrappingPolicy(WrappingPolicy policy)
	{
		wrappingX = wrappingY = policy;
	}

	void Texture::setMipmapEnabled(bool isEnabled)
	{
		mipmapEnabled = isEnabled;
	}

	void Texture::setSamplingPolicy(SamplingPolicy min, SamplingPolicy mag)
	{
		sampingMin = min;
		sampingMag = mag;
	}

	void Texture::setSamplingPolicy(SamplingPolicy policy)
	{
		sampingMin = sampingMag = policy;
	}

	void Texture::setMipmapSamplingPolicy(SamplingPolicy policy)
	{
		sampingMipmap = policy;
	}

	void Texture::setBorderColor(Vec4 color)
	{
		borderColor = color;
	}

	void Texture::bind(unsigned int texUnit)
	{
		if (!data)
		{
			std::cerr << "ERROR | XGL::Texture::bind(int) : No image data.\n";
			throw NO_IMAGE_DATA;
		}
		textureUnit = texUnit;
		if (!handle)
			glGenTextures(1, &handle);
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		glBindTexture(GL_TEXTURE_2D, handle);
		
		switch (wrappingX)
		{
			case REPEAT:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); break;
			case MIRRORED_REPEAT:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); break;
			case CLAMP_TO_EDGE:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); break;
			case CLAMP_TO_BORDER:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); break;
		}

		switch (wrappingY)
		{
			case REPEAT:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); break;
			case MIRRORED_REPEAT:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT); break;
			case CLAMP_TO_EDGE:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); break;
			case CLAMP_TO_BORDER:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER); break;
		}
		
		if (!mipmapEnabled)
		{
			switch (sampingMin)
			{
				case NEAREST:
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); break;
				case LINEAR:
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); break;
			}
			switch (sampingMag)
			{
				case NEAREST:
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); break;
				case LINEAR:
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); break;
			}
		}
		else
		{
			switch (sampingMin)
			{
				case NEAREST:
					switch (sampingMipmap)
					{
						case NEAREST:
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST); break;
						case LINEAR:
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR); break;
					} break;
				case LINEAR:
					switch (sampingMipmap)
					{
						case NEAREST:
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); break;
						case LINEAR:
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); break;
					} break;
			}
			switch (sampingMag)
			{
				case NEAREST:
					switch (sampingMipmap)
					{
						case NEAREST:
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST); break;
						case LINEAR:
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR); break;
					} break;
				case LINEAR:
					switch (sampingMipmap)
					{
						case NEAREST:
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST); break;
						case LINEAR:
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR); break;
					} break;
			}
		}

		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor.getData());
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		if (mipmapEnabled)
			glGenerateMipmap(GL_TEXTURE_2D);
	}
}