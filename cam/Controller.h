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
	Controller(int currContIndex);
	~Controller();
	void UpdateState(int i);
	int getAccelleration(int i);
	bool getBrake(int i);
	int getDirectional(int i);
	int MAX_NUM_CONTROL = 4;
	int i;
	void setState(int i, int stateSet);
private:
	XINPUT_STATE state;
	XINPUT_STATE getState(int i);
};