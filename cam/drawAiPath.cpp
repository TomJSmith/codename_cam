#include "drawAiPath.h"



drawAiPath::drawAiPath()
{
}


drawAiPath::~drawAiPath()
{
}

void drawAiPath::setupDraw(vector<vec3> aiPath, vec3 carPos)
{

	
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
	
}
vector<vec3> drawAiPath::getPath()
{
	return pathToDraw;
}