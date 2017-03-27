#include "ImageShader.h"

ImageShader::ImageShader(const char *filename, size_t layer) :
	Shader({
		{GL_VERTEX_SHADER, "image.vert"}, 
		{GL_FRAGMENT_SHADER, "image.frag"}
    }),
	texture_(filename),
	layer_(layer)
{}
//
//ImageShader::ImageShader(unsigned char *data, size_t layer) :
//	Shader({
//		{GL_VERTEX_SHADER, "image.vert"},
//		{GL_FRAGMENT_SHADER, "image.frag"}
//	}),
//	texture_(data),
//	layer_(layer)
//{}

void ImageShader::Setup(const Renderer::MeshData &data, const mat4 &projection)
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

	glUniform1ui(glGetUniformLocation(*program_, "layer"), (GLuint)layer_);
	glUniform1ui(glGetUniformLocation(*program_, "max_layers"), (GLuint)64); // TODO hacky hacky
}