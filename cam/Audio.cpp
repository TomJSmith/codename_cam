#include "Audio.h"


bool Audio::init() 
{ 
	sound1 = "\\test.wav";

	char* currDir;
	
	// Get the current working directory:   
	if ((currDir = _getcwd(NULL, 0)) == NULL)
	{
		perror("_getcwd error");
		return false;
	}
	else
	{
		sound1 = currDir + sound1;
		
		free(currDir);
		return true;
	}

}

void Audio::play()
{
	PlaySound(sound1.c_str(), NULL, SND_FILENAME || SND_ASYNC);
}
