#include "drawAiPath.h"


#include <iostream>
drawAiPath::drawAiPath()
{
}


drawAiPath::~drawAiPath()
{
}

void drawAiPath::setupDraw(vector<vec3> aiPath, vec3 carPos)
{
	vec3 color = vec3(1.0f, 1.0f, 1.0f);
	colorPath.push_back(color);
	colorPath.push_back(color);
	pathToDraw.push_back(carPos);
	pathToDraw.push_back(aiPath[0]);
	for (int i = 0; i < aiPath.size()-1; i++)
	{
		colorPath.push_back(color);
		colorPath.push_back(color);
		pathToDraw.push_back(aiPath[i]);
		pathToDraw.push_back(aiPath[i+1]);
	}

	/*
	if (aiPath.size() > 0)
	{
		vec3 adjustVec;
		pathToDraw.push_back(carPos);
		carPos.x += 0.5;
		pathToDraw.push_back(carPos);
		pathToDraw.push_back(aiPath[0]);

		for (int i = 0; i < aiPath.size() - 1; i++)
		{
			pathToDraw.push_back(aiPath[i]);
			adjustVec = aiPath[i];
			adjustVec.x += 0.5f;
			pathToDraw.push_back(adjustVec);
			pathToDraw.push_back(aiPath[i+1]);
		}
	}
	*/


	
}

#ifdef DEBUG
std::vector<Renderer::MeshData> drawAiPath::GetDebugMeshData()
{
	static Shader &shader = Shader::Load("passthrough.vert", "passthrough.frag");

	static std::vector<GLuint> vertexVbos;
	static std::vector<GLuint> colourVbos;
	static std::vector<Renderer::MeshData> ret;

	glDeleteBuffers((GLsizei)vertexVbos.size(), vertexVbos.data());
	glDeleteBuffers((GLsizei)colourVbos.size(), colourVbos.data());

	vertexVbos.clear();
	colourVbos.clear();

	for (auto &i : ret) {
		glDeleteVertexArrays(1, &i.vao);
	}

	ret.clear();

	// TODO there are points, triangles and text we could render too...
	// probably the exact same way as this
		vector<vec3> aiPathToDraw;

		aiPathToDraw.clear();
		pathToDraw.clear();
		colorPath.clear();

		vec3 carPos = vec3(0.0f, 2.0f, -10.0f);

		aiPathToDraw.push_back(vec3(0.0f, 2.0f, 10.0f));
		aiPathToDraw.push_back(vec3(-10.0f, 2.0f, 10.0f));
		aiPathToDraw.push_back(vec3(-10.0f, 2.0f, -10.0f));
		aiPathToDraw.push_back(vec3(0.0f, 2.0f, -10.0f));
		setupDraw(aiPathToDraw, carPos);

		std::cout << "Path to draw array size"<< pathToDraw.size() << std::endl;
		GLuint vertexBuffer = 0;
		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * pathToDraw.size() , pathToDraw.data(), GL_STATIC_DRAW);

		GLuint colourBuffer = 0;
		glGenBuffers(1, &colourBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * colorPath.size(), colorPath.data(), GL_STATIC_DRAW);

		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		vertexVbos.push_back(vertexBuffer);
		colourVbos.push_back(colourBuffer);

		ret.push_back(Renderer::MeshData{
			vao,
			shader.Program(),
			2,
			GL_LINES,
			mat4(1.0)
		});
	

	return ret;
}
#endif // #ifdef DEBUG