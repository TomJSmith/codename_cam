#include "TextShader.h"

TextShader::TextShader(unsigned char *data, int width, int height, size_t layer)
	: Shader({
		{GL_VERTEX_SHADER, "image.vert"},
		{GL_FRAGMENT_SHADER, "text.frag"}
	}),
	texture_(data, width, height),
	layer_(layer)
{
}

void TextShader::Setup(const Renderer::MeshData &data, const mat4 &projection)
{
	glUseProgram(*program_);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_.GetTexture());
	glUniform1i(glGetUniformLocation(*program_, "tex"), 0);

	//glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	mat3 t(data.modelMatrix);
	glUniformMatrix3fv(
		glGetUniformLocation(*program_, "transform"),
		1,
		GL_FALSE,
		&t[0][0]
	);

	//GLfloat c[3] = { 1.0f, 1.0f, 1.0f }; // TODO

	glUniform1ui(glGetUniformLocation(*program_, "layer"), (GLuint)layer_);
	glUniform1ui(glGetUniformLocation(*program_, "max_layers"), (GLuint)64); // TODO hacky hacky
	//glUniform3fv(glGetUniformLocation(*program_, "textcolour"), 1, c);
}
