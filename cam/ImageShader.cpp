#include "ImageShader.h"

ImageShader::ImageShader(const char *filename) :
	Shader({
		{GL_VERTEX_SHADER, "image.vert"}, 
		{GL_FRAGMENT_SHADER, "image.frag"}
    }),
	texture_(filename)
{}

void ImageShader::Setup(const Renderer::MeshData &data, const mat4 &projection)
{
	glUseProgram(*program_);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_.GetTexture());
	glUniform1i(glGetUniformLocation(*program_, "tex"), 0);

	mat3 t(data.modelMatrix);
	glUniformMatrix3fv(
		glGetUniformLocation(*program_, "transform"),
		1,
		GL_FALSE,
		&t[0][0]
	);
}