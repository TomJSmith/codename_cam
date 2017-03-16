#pragma once

#include "Component.h"

#include <functional>

#include "Events.h"
#include "ImageShader.h"

class Image : public Component
{
public:
	Image(const char *filename);

	void Destroy() override;

protected:
	void RegisterHandlers() override;

private:
	ImageShader shader_;
	std::function<void(Events::Render)> handler_;
	GLuint vao_;
};

