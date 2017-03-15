#include "ModelShader.h"

#include "Renderer.h"

ModelShader::ModelShader() :
	Shader({
		{ GL_VERTEX_SHADER, "passthrough.vert" },
		{ GL_FRAGMENT_SHADER, "passthrough.frag" }
    })
{}

void ModelShader::Setup(const Renderer::MeshData &data, const mat4 &projection)
{
	glUseProgram(*program_);

	auto mvp = projection * data.modelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(*program_, "mvp"), 1, GL_FALSE, &mvp[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(*program_, "projection"), 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(*program_, "modelview"), 1, GL_FALSE, &data.modelMatrix[0][0]);
}
