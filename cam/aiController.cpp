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
void aiController::setLeft(int dir)
{
	if (dir == 1)
		left = true;
	else
		left = false;
}
bool aiController::getLeft()
{
	return left;
}

void aiController::setBrake(int dir)
{
	if (dir == 1)
		brake = true;
	else
		brake = false;
}
bool aiController::getBrake()
{
	return brake;
}
void aiController::setReverse(int dir)
{
	if (dir == 1)
		reverse = true;
	else
		reverse = false;
}
bool aiController::getReverse()
{
	return reverse;
}
