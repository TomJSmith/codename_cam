#pragma once
#include "Controller.h"
class aiController : public Controller
{

public:
	aiController();
	~aiController();

	bool getBrake() override;
	int getAcceleration() override;
	int getDirectional() override;
	void UpdateState() override;

	void setRight(int dir);
	void setLeft(int dir);
	void setBrake(int dir);
	void setReverse(int dir);
	void setAccel(int dir);

private:
	bool right = false, left = false, none = false, brake = false, reverse = false, accel = false;
};

