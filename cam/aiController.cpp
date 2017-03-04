#include "aiController.h"
#include <iostream>

aiController::aiController(int currIndex) : Controller(i)
{

	ZeroMemory(&stateAi, sizeof(XINPUT_STATE));
	if (XInputGetState(1, &stateAi) == ERROR_SUCCESS) {
		std::cout << "Controller is connected" << std::endl;
	}
	else
	{
		std::cout << "Controller is not connected" << std::endl;
	}
	i = currIndex;
	std::cout << "AI CONTROLLER CONSTRUCTOR" << std::endl;
}

aiController::~aiController()
{

}

void aiController::setRight(int dir)
{
	if (dir == 1)
		right = true;
	else
		right = false;
}
bool aiController::getRight()
{
	return right;
}