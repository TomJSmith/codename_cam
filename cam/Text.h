#pragma once

#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Component.h"
#include "Image.h"
#include "Physics.h"

class Text : public Component
{
public:
	// TODO add layers to this like images
	// and figure out this transparency stuff properly
	Text(std::string text, const std::string &font, vec2 pos, size_t fontsize);
	~Text();

	virtual void RegisterHandlers() override;
	static void Initialize();

	vec2 GetPosition() { return pos_; }
	void SetPosition(const vec2 &pos);
	std::string GetText() { return text_; }
	void SetText(std::string text);

private:
	void CreateText();

	std::string text_;
	vec2 pos_;

	FT_Face face_;

	std::weak_ptr<Entity> child_;

	static FT_Library library_;
};

