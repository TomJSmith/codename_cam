#include "Controller.h"
#include <iostream>


#include <stdexcept>
#include "windows.h"
#include "Xinput.h"


Controller::Controller(int currContIndex) {
	//ZeroMemory(&state, sizeof(XINPUT_STATE));
	for (int i = 0; i < MAX_NUM_CONTROL; i++)
	{
		if (XInputGetState(i, &state) == ERROR_SUCCESS) {
			std::cout << "Controller is connected" << std::endl;
		}
		else
		{
			std::cout << "Controller is not connected" << std::endl;
		}
	}
	i = currContIndex;
}


Controller::~Controller() {
}

XINPUT_STATE Controller::getState(int i) {
//	ZeroMemory(&state, sizeof(XINPUT_STATE));
	
	XInputGetState(i, &state);
	return state;
}
void Controller::setState(int i, int stateSet) {
	//	ZeroMemory(&state, sizeof(XINPUT_STATE));

	XInputGetState(i, &state);

	
	//return state;
}

void Controller::UpdateState(int i) {
	//ZeroMemory(&state, sizeof(XINPUT_STATE));
	XInputGetState(i, &state);
}

int Controller::getAccelleration(int i) {
	XInputGetState(i, &state);
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
		return C_FAST;
	}
	else if (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) {
		return C_REVERSE;
	}
	else {
		return C_NEUTRAL;
	}
}

bool Controller::getBrake(int i) {
	XInputGetState(i, &state);
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
		return true;
	}
	else {
		return false;
	}
}

int Controller::getDirectional(int i) {
	XInputGetState(i, &state);
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