#pragma once
#include "Controller.h"
class aiController : public Controller
{

public:
	aiController(int num);
	~aiController();

	bool getBrake() override;
	int getAcceleration() override;
	float getDirectional() override;
	void UpdateState() override;

	void setDirection(float dir);
	void setBrake(int dir);
	void setReverse(int dir);
	void setAccel(int dir);

private:
	bool none = false, brake = false, reverse = false, accel = false, doOnce = false;
	float direction = 0.0f;
	int aiControllerNumber;
};

