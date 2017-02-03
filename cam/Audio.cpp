#include "Audio.h"

Mix_Music *bgm; //= Mix_LoadMUS("test.wav"); //Music longer than 10 seconds should be Mix_Music General rule
Mix_Chunk *soundEffect; //= Mix_LoadWAV("test2.wav");

bool Audio::init() 
{ //Initialize all SDL subsystems 
	if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 ) { return false; } //Set up the screen 

	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) == -1 ) 
	{ 
		std::cout << "Error: " << Mix_GetError() << std::endl;
		return false; 
	} //Set the window caption 
	return true; 
}

void Audio::play()
{
	bgm = Mix_LoadMUS("test.wav");
	//soundEffect = Mix_LoadWAV("test2.wav");

}
