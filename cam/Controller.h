#pragma once

#include "System.h"

class Controller {
public:
	Controller();
	~Controller();
	XINPUT_STATE getState();
private:
	XINPUT_STATE state;
};