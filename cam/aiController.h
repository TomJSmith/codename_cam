#pragma once
#include "Controller.h"
class aiController : public Controller
{

public:
	aiController();
	~aiController();
private:
	XINPUT_STATE state;

};

