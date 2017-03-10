#pragma once
#include <vector>
#include <glm/vec3.hpp>
using namespace std;
using namespace glm;
class drawAiPath
{
public:
	drawAiPath();
	~drawAiPath();
	vector<vec3> pathToDraw;
	void setupDraw(vector<vec3> aiPath, vec3 carPos);
	vector<vec3> getPath();
private:
	vector<vec3> pathToDraw;
	vector<vec3> colorPath;
	vector<vec3> fakeNorm;
};

