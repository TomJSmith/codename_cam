
#pragma once

#include "System.h"

#include <cstdlib>
#include <direct.h>
#include <cstring>
#include <iostream>

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
	void playAudio(int choice);
	void cleanUpAudio(wavFile wav);

private:
	void checkError();
	char* Audio::findPathToFile();
	void list_audio_devices(const ALCchar *devices);
	ALuint sourceSetup(ALuint source);
	ALenum formatWav(wavFile wav);
	wavFile openWavFile(string fileName, wavFile toPlay);
	wavFile wav, wav2, wav3, wav4; //creative names later
	ALCcontext *context;
	ALCdevice* Device; // select the "preferred device"
};



