#pragma once
#include "windows.h"
#include "Xinput.h"

class Controller {
public:
	Controller();
	~Controller();
	XINPUT_STATE getState();
private:
	XINPUT_STATE state;
};