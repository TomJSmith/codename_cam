#include "ModelShader.h"

#include "Renderer.h"

ModelShader::ModelShader() :
	Shader({
		{ GL_VERTEX_SHADER, "passthrough.vert" },
		{ GL_FRAGMENT_SHADER, "passthrough.frag" }
})
{}

ModelShader::ModelShader(const char* filename) :
	Shader({
		{ GL_VERTEX_SHADER, "passthrough.vert" },
		{ GL_FRAGMENT_SHADER, "passthrough.frag" }
    }),
	texture_(filename)
{}

void ModelShader::Setup(const Renderer::MeshData &data, const mat4 &projection)
{
	glUseProgram(*program_);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_.GetTexture());

	auto mvp = projection * data.modelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(*program_, "mvp"), 1, GL_FALSE, &mvp[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(*program_, "projection"), 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(*program_, "modelview"), 1, GL_FALSE, &data.modelMatrix[0][0]);
	glUniform1i(glGetUniformLocation(*program_, "model_texture"), 0);
}
