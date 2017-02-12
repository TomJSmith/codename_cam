#include "Controller.h"
#include <iostream>


#include <stdexcept>
#include "windows.h"
#include "Xinput.h"


Controller::Controller() {
	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	if (XInputGetState(0, &state) == ERROR_SUCCESS){
		std::cout << "Controller is connected";
	}
	else
	{
		std::cout << "Controller is not connected";
	}
}


Controller::~Controller() {
}

XINPUT_STATE Controller::getState() {
	ZeroMemory(&state, sizeof(XINPUT_STATE));
	XInputGetState(0, &state);
	return state;
}