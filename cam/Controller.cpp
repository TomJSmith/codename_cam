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
	float y = state.Gamepad.sThumbLY;
	if ((y > 7000) & (y < 20000)) {
		return C_SLOW;
	}
	else if (y >= 20000) {
		return C_FAST;
	}
	else if (y < -5000) {
		return C_REVERSE;
	}
	else {
		return C_NEUTRAL;
	}
}

bool Controller::getBrake() {
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
		std::cout << "BRAKING";
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