#pragma once

#include "System.h"

#define C_DOWN 7
#define C_UP 6
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

	virtual void UpdateState();
	virtual int getAcceleration();
	virtual bool getBrake();
	virtual bool getSelect();
	virtual bool getSecondary();
	virtual float getDirectional();

private:
	int i;
	XINPUT_STATE state;
};