#include "Controller.h"
#include <iostream>


#include <stdexcept>
#include "windows.h"
#include "Xinput.h"

Controller::Controller(int currContIndex) {
	//ZeroMemory(&state, sizeof(XINPUT_STATE));
	//for (int i = 0; i < MAX_NUM_CONTROL; i++)
	i = currContIndex;
	//{
	if (XInputGetState(i, &state) == ERROR_SUCCESS) {
		std::cout << "Controller is connected" << std::endl;
	}
	else
	{
		std::cout << "Controller is not connected" << std::endl;
	}
	//}
}


Controller::~Controller() {
}

void Controller::UpdateState() {
	//ZeroMemory(&state, sizeof(XINPUT_STATE));
	XInputGetState(i, &state);
}

int Controller::getAcceleration() {
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
	else if (state.Gamepad.sThumbLY < -10000) {
		return C_DOWN;
	}
	else if (state.Gamepad.sThumbLY > 10000) {
		return C_UP;
	}
	else {
		return C_NO_DIRECTION;
	}
}

bool Controller::getSelect() {
	return (state.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0;
}

bool Controller::getSecondary() {
	return (state.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0;
}