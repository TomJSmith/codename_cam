#pragma once

#include "System.h"

#include "Component.h"

class Texture : public Component
{
public:
	Texture();
	Texture(const char *filename);
	~Texture();

	GLuint GetTexture() { return texture_; }

private:
	int width_;
	int height_;
	int ncomponents_;
	GLuint texture_;

	unsigned char *data_;
};

