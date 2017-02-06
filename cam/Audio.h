#pragma once
#include <SDL/include/SDL.h>

#include <SDL/include/SDL_mixer.h>

#include <iostream>

/*
#include <SDL/include/SDL_stdinc.h>
#include <SDL/include/SDL_rwops.h>
#include <SDL/include/SDL_audio.h>
#include <SDL/include/SDL_endian.h>
#include <SDL/include/SDL_version.h>
#include <SDL/include/begin_code.h>
#include <SDL/include/close_code.h>
*/

class Audio
{
public:
	bool init();
	void play();
	
};

