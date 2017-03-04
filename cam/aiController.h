#pragma once
#include "Controller.h"
class aiController : public Controller
{

public:
	aiController(int currIndex); //Set the currIndex to anything >= 4
	~aiController();
	void setRight(int dir);
	bool right =false, left=false, none=false, brake=false, reverse=false;
	bool getRight();

private:
	XINPUT_STATE stateAi;

};

