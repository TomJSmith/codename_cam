#include "aiController.h"
#include <iostream>

aiController::aiController(int num)
	: Controller(aiControllerNumber)
{
	aiControllerNumber = num;
}

aiController::~aiController()
{}

int aiController::getAcceleration() {
	if (reverse) return C_REVERSE;
	if (accel) return C_FAST;
	return C_NEUTRAL; // TODO
}

float aiController::getDirectional() {
	return direction;
}

bool aiController::getBrake()
{
	return brake;
}

void aiController::UpdateState() { /* do nothing, this is for xbox controllers */ }

void aiController::setDirection(float dir)
{
	direction = dir;
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
	doOnce = false;
	if (dir == 1)
		accel = true;
	else
		accel = false;
}


void aiController::setReverse(int dir)
{
	if (dir == 1)
	{
		reverse = true;
	}
	else
		reverse = false;
	
}
