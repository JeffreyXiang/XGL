#ifndef XGL_TEXTURE_H
#define XGL_TEXTURE_H

#include <stb_image.h>
#include <glad/glad.h>
#include <Math/Vector.h>

namespace XGL
{
	class Texture
	{
		public:
			enum ERROR { FILE_OPEN_FAIL, NO_IMAGE_DATA, NOT_GENERATED };
			enum WrappingPolicy { REPEAT, MIRRORED_REPEAT, CLAMP_TO_EDGE, CLAMP_TO_BORDER };
			enum SamplingPolicy { NEAREST, LINEAR };

		private:
			// --- image info ---
			unsigned char* data;
			int width;
			int height;
			int channel;

			// --- GL info ---
			unsigned int handle;

			// --- property ---
			bool mipmapEnabled;
			WrappingPolicy wrappingX;
			WrappingPolicy wrappingY;
			SamplingPolicy sampingMin;
			SamplingPolicy sampingMag;
			SamplingPolicy sampingMipmap;
			Vec4 borderColor;


		public:
			Texture() : data(NULL), handle(0), mipmapEnabled(true),
				wrappingX(REPEAT), wrappingY(REPEAT),
				sampingMin(LINEAR), sampingMag(LINEAR), sampingMipmap(LINEAR),
				borderColor(0, 0, 0, 1) {}
			Texture(const char* filename) : Texture() { load(filename); }
			~Texture() { stbi_image_free(data); glDeleteTextures(1, &handle); }

			unsigned char* getData() { return data; }
			int getWidth() { return width; }
			int getHeight() { return height; }
			unsigned int getHandle() { return handle; }

			void load(const char* filename);
			void setMipmapEnabled(bool isEnabled);
			void setWrappingPolicy(WrappingPolicy x, WrappingPolicy y);
			void setWrappingPolicy(WrappingPolicy policy);
			void setSamplingPolicy(SamplingPolicy min, SamplingPolicy mag);
			void setSamplingPolicy(SamplingPolicy policy);
			void setMipmapSamplingPolicy(SamplingPolicy policy);
			void setBorderColor(Vec4 color);

			bool isGenerated() { return handle; }
			void generate();
			void bind(unsigned int texUnit);
	};
}

#endif // ! XGL_TEXTURE_H
