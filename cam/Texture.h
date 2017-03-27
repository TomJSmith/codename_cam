#pragma once

#include "System.h"

#include "Component.h"

class Texture : public Component
{
public:
	Texture();
	Texture(const char *filename);
	Texture(unsigned char *data, int width, int height);
	~Texture();

	GLuint GetTexture() { return texture_; }

private:
	int width_;
	int height_;
	int ncomponents_;
	GLuint texture_;

	void Initialize(unsigned char *data, GLenum type);

	//unsigned char *data_;
};

