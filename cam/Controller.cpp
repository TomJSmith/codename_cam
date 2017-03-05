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

void Controller::UpdateState() {
	ZeroMemory(&state, sizeof(XINPUT_STATE));
	XInputGetState(0, &state);
}

int Controller::getAccelleration() {
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
		return C_FAST;
	}
	else if (state.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
		return C_REVERSE;
	}
	else {
		return C_NEUTRAL;
	}
}

bool Controller::getBrake() {
	// bRightTrigger = 0..255
	// if more than half pressed, brake
	if (state.Gamepad.bRightTrigger > 127) {
		return true;
	}
	else {
		return false;
	}
}

int Controller::getDirectional() {
	if (state.Gamepad.sThumbLX < -10000) {
		return C_RIGHT;
	}
	else if (state.Gamepad.sThumbLX > 10000) {
		return C_LEFT;
	}
	else {
		return C_NO_DIRECTION;
	}
}