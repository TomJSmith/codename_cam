#pragma once
#include <Windows.h>	
#include <MMSystem.h>
#include <direct.h>
#include <cstring>
#include <iostream>



class Audio
{
public:
	bool init();
	void play();
private:
	std::string sound1;
	std::string sound2;
};

