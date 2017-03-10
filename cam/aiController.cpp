#include "aiController.h"
#include <iostream>

aiController::aiController()
	: Controller(4)
{}

aiController::~aiController()
{}

int aiController::getAcceleration() {
	if (reverse) return C_REVERSE;
	if (accel) return C_FAST;
	return C_NEUTRAL; // TODO
}

int aiController::getDirectional() {
	if (right) return C_RIGHT;
	if (left) return C_LEFT;
	return C_NO_DIRECTION;
}

bool aiController::getBrake()
{
	return brake;
}

void aiController::UpdateState() { /* do nothing, this is for xbox controllers */ }

void aiController::setRight(int dir)
{
	if (dir == 1)
		right = true;
	else
		right = false;
}

void aiController::setLeft(int dir)
{
	if (dir == 1)
		left = true;
	else
		left = false;
}

void aiController::setBrake(int dir)
{
	if (dir == 1)
		brake = true;
	else
		brake = false;
}
void aiController::setAccel(int dir)
{
	if (dir == 1)
		accel = true;
	else
		accel = false;
}


void aiController::setReverse(int dir)
{
	if (dir == 1)
		reverse = true;
	else
		reverse = false;
}
