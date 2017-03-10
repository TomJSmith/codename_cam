#pragma once
#include <vector>
#include <glm/vec3.hpp>
#include "Renderer.h"
#include "Shader.h"

using namespace std;
using namespace glm;
class drawAiPath
{
public:
	drawAiPath();
	~drawAiPath();
	vector<vec3> pathToDraw;
	void setupDraw(vector<vec3> aiPath, vec3 carPos);
	void initRender();
	vector<vec3> getPath();
	GLuint vertBuffer;
	Shader pathShader;
private:
	vector<vec3> pathToDraw;
	vector<vec3> colorPath;
	vector<vec3> fakeNorm;
};

