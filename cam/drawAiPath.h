#pragma once
#include <vector>
#include <glm/vec3.hpp>
#include "Renderer.h"
#include "Shader.h"

using namespace std;
class drawAiPath
{
public:
	drawAiPath();
	~drawAiPath();
	void setupDraw(vector<vec3> aiPath, vec3 carPos);
	vector<Renderer::MeshData> GetDebugMeshData();
private:
	vector<vec3> pathToDraw;
	vector<vec3> colorPath;
};

