
#pragma once

#include "System.h"

#include <cstdlib>
#include <direct.h>
#include <cstring>
#include <iostream>
#include <glm\vec3.hpp>

class Entity;

using namespace std;

struct wavFile {

	char type[4];
	DWORD size, chunkSize;
	short formatType, channels;
	DWORD sampleRate, avgBytesPerSec;
	short bytesPerSample, bitsPerSample;
	DWORD dataSize;
	ALuint buffer;
	ALuint source;
	ALenum format;
	unsigned char* songBuf;

};
class Audio
{
public:
	Audio();
	~Audio();
	void initAudio();
	void playAudio(int choice, ALuint source, int prevChoice, bool isPlayer, glm::vec3 soundPos, glm::vec3 forwardVec);
	void cleanUpAudio(wavFile wav);
	ALuint sourceSetup(ALuint source, float vol, glm::vec3 pos, bool backSound);
	
	void playSounds(Entity &entity);

private:
	void checkError();
	char* Audio::findPathToFile();
	void list_audio_devices(const ALCchar *devices);
	glm::vec3 prevLisPos;
	bool first = true;
	ALenum formatWav(wavFile wav);
	wavFile openWavFile(string fileName, wavFile toPlay);
	wavFile background;
	wavFile horn, speedUp, speedDown, idle;
	ALCcontext *context;
	ALCdevice* Device; // select the "preferred device"
};



