#pragma once

#include "System.h"

#define C_RIGHT 5
#define C_LEFT 4
#define C_NO_DIRECTION 3
#define C_FAST 2
#define C_SLOW 1
#define C_NEUTRAL 0
#define C_REVERSE -1


class Controller {
public:
	Controller();
	~Controller();
	void UpdateState();
	int getAccelleration();
	bool getBrake();
	int getDirectional();
private:
	XINPUT_STATE state;
	XINPUT_STATE getState();
};