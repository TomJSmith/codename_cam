#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "Util.h"

Texture::Texture(const char *filename)
{
	auto fullfilename = Util::TextureDirectory + filename;
	auto data = stbi_load(fullfilename.c_str(), &width_, &height_, &ncomponents_, STBI_rgb_alpha);

	glGenTextures(1, &texture_);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)width_, (GLsizei)height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	for (int row = 0; row < height_; ++row) {
		int offset = (height_ - row - 1) * width_ * 4;
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, row, width_, 1, GL_RGBA, GL_UNSIGNED_BYTE, data + offset);
	}

	stbi_image_free(data_);
}

Texture::~Texture()
{
	glDeleteTextures(1, &texture_);
}