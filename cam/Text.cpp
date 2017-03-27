#include "Text.h"

#include "Entity.h"
#include "Util.h"

FT_Library Text::library_;

Text::Text(std::string text, const std::string &font, vec2 pos, size_t fontsize) :
	text_(std::move(text)),
	pos_(std::move(pos))
{
	auto fontfile = Util::FontDirectory + font + ".ttf";
	auto err = FT_New_Face(library_, fontfile.c_str(), 0, &face_);
	if (err == FT_Err_Unknown_File_Format) {
		throw std::runtime_error(std::string("unrecognized font file ") + fontfile);
	}
	else if (err) {
		throw std::runtime_error(std::string("error loading font file ") + fontfile);
	}

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	// waaat is this f26dot6 type srsly
	err = FT_Set_Char_Size(face_, 0, (FT_F26Dot6)fontsize * 64, viewport[2], viewport[3]);
	if (err) {
		throw std::runtime_error(std::string("error setting font size for ") + fontfile);
	}
}

Text::~Text()
{
	if (auto child = child_.lock()) child->Destroy();
}

void Text::Initialize()
{
	if (FT_Init_FreeType(&library_))
	{
		throw std::runtime_error("failed to initalize freetype");
	}
}

void Text::RegisterHandlers()
{
	CreateText();
}

void Text::CreateText()
{
	if (auto child = child_.lock()) {
		child->Destroy();
	}

	child_ = Entity::Create(entity_);
	auto child = child_.lock();

	vec2 pos = pos_;

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	std::vector<std::shared_ptr<Image>> images;

	for (const auto &c : text_) {
		FT_UInt i = FT_Get_Char_Index(face_, c);
		if (FT_Load_Glyph(face_, i, FT_LOAD_DEFAULT)) {
			throw std::runtime_error("failed to load glyph");
		}

		if (FT_Render_Glyph(face_->glyph, FT_RENDER_MODE_NORMAL)) {
			throw std::runtime_error("failed to render glyph");
		}

		auto g = face_->glyph;
		auto bmp = g->bitmap;

		vec2 currpos = pos + vec2((float)g->bitmap_left / viewport[2], (float)g->bitmap_top / viewport[3]);
		vec2 size((float)bmp.width / viewport[2], (float)bmp.rows / viewport[3]);

		images.push_back(std::make_shared<Image>(bmp.buffer, bmp.width, bmp.rows, pos, size));

		pos += vec2((float)(g->advance.x >> 6) / viewport[2], (float)(g->advance.y >> 6) / viewport[3]);
	}

	// Sort these in reverse, so that later letters are drawn first, because opengl
	// does transparency strangely... the recommendation is to sort transparent images
	// from far to near, then far things are properly seen through near things
	//
	// This means later letters overlap the earlier letters, which isn't quite perfect but
	// it's much better than earlier letters overlapping later ones because fonts are
	// usually designed to be drawn with later letters overlapping earlier ones
	std::reverse(images.begin(), images.end());
	for (auto &i : images) {
		child->AddComponent(std::move(i));
	}
}

void Text::SetPosition(const vec2 &pos)
{
	pos_ = pos;
	CreateText();
}

void Text::SetText(std::string text)
{
	text_ = std::move(text);
	CreateText();
}