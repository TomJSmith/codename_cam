#include "Audio.h"

#include "Util.h"
#include "Entity.h"
Audio::Audio()
{
}


Audio::~Audio()
{
	cleanUpAudio(background);
	cleanUpAudio(idle);
	cleanUpAudio(speedDown);
	cleanUpAudio(speedUp);
	cleanUpAudio(horn);

}

char* Audio::findPathToFile()
{


	char* currDir;

	// Get the current working directory:   
	if ((currDir = _getcwd(NULL, 0)) == NULL)
	{
		perror("_getcwd error");
		return false;
	}
	else
	{
		return currDir;
	}
}

void Audio::initAudio()
{
	// Initialization

	/*Open a device(In this case the default device)*/
	char* pathToWav = findPathToFile();

	Device = alcOpenDevice(nullptr);
	if (!Device)
	{
		perror("Error opening AL device");

	}
	/*Check to see if there are other devices to access*/
	ALboolean enumeration;
	enumeration = alcIsExtensionPresent(nullptr, "ALC_ENUMERATION_EXT");
	if (enumeration == AL_FALSE)
	{
		cout << "Enumeration not supported" << endl;
	}
	else
	{
		cout << "Lets Enumerate" << endl;
		list_audio_devices(alcGetString(NULL, ALC_DEVICE_SPECIFIER));
	}

	/*Reset the error stack has a depth of 1 error.  Do this to help determine where the errors are*/
	checkError();


	/*Initializing a context to render an audio scene*/

	context = alcCreateContext(Device, NULL);
	if (!alcMakeContextCurrent(context))
		cout << "Failed to make context current" << endl;

	checkError();

	/*Configuring a listener*/
	ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };

	alListener3f(AL_POSITION, 0, 0, 1.0f);
	checkError();

	alListener3f(AL_VELOCITY, 0, 0, 0);
	checkError();

	alListenerfv(AL_ORIENTATION, listenerOri);
	checkError();

	/*Source Generation creating an audio source object which is the origin of the sound*/

	alGenSources(1, &horn.source);
	checkError();

	alGenSources(1, &speedUp.source);
	checkError();

	alGenSources(1, &speedDown.source);
	checkError();

	alGenSources(1, &idle.source);
	checkError();

	alGenSources(1, &background.source);
	checkError();



	background.source = sourceSetup(background.source, 0.2f, glm::vec3(0.0f, 0.0f, 0.0f), false);
	horn.source = sourceSetup(horn.source, 0.1f, glm::vec3(0.0f, 0.0f, 0.0f), false);
	speedUp.source = sourceSetup(speedUp.source, 0.3f, glm::vec3(0.0f, 0.0f, 0.0f), false);
	speedDown.source = sourceSetup(speedDown.source, 0.3f, glm::vec3(0.0f, 0.0f, 0.0f), false);
	idle.source = sourceSetup(idle.source, 0.3f, glm::vec3(0.0f, 0.0f, 0.0f), false);


	/*Buffer Generation this holds the raw audio stream*/

	alGenBuffers(1, &horn.buffer);
	checkError();

	alGenBuffers(1, &speedUp.buffer);
	checkError();

	alGenBuffers(1, &speedDown.buffer);
	checkError();

	alGenBuffers(1, &idle.buffer);
	checkError();

	alGenBuffers(1, &background.buffer);
	checkError();


	string backgroundLoc = Util::AudioDirectory + "Background.wav";
	string hornLoc = Util::AudioDirectory + "Horn.wav";
	string idleLoc = Util::AudioDirectory + "Idle.wav";
	string speedUpLoc = Util::AudioDirectory + "SpeedingUp.wav";
	string speedDownLoc = Util::AudioDirectory + "SlowingDown.wav";


	backgroundLoc = pathToWav + backgroundLoc;

	hornLoc = pathToWav + hornLoc;
	idleLoc = pathToWav + idleLoc;
	speedUpLoc = pathToWav + speedUpLoc;
	speedDownLoc = pathToWav + speedDownLoc;

	//cout << firstWav << endl;
	background = openWavFile(backgroundLoc, background);
	horn = openWavFile(hornLoc, horn);
	idle = openWavFile(idleLoc, idle);
	speedUp = openWavFile(speedUpLoc, speedUp);
	speedDown = openWavFile(speedDownLoc, speedDown);


	background.format = formatWav(background);

	horn.format = formatWav(horn);
	idle.format = formatWav(idle);
	speedUp.format = formatWav(speedUp);
	speedDown.format = formatWav(speedDown);


}

/*TODO find a better way to figure out if something is playing or not, otherwise there is a bunch of errors being printed
I think its to do with it constantly changing toPlay to one of the wav files need to find a better way to set toPlay to a wav file
*/

void Audio::playAudio(int choice, ALuint source, int prevChoice)
{
	wavFile toPlay;
	//if(choice == 0)
		//cout << "MY CHOICE: " << choice << endl;
	
	switch (choice)
	{
	case 5:
		toPlay = background;
		break;
	case 1:
		toPlay = idle;
		break;
	case 2:
		toPlay = speedUp;
		break;
	case 3:
		toPlay = speedDown;
		break;
	case 4:
		toPlay = horn;
		break;
	default:
		cout << "Invalid song choice" << endl;
		return;

	}
	//toPlay->source = source;
	/*This just forces it to play one song until the song is finished*/
	ALint source_state;
	//cout << "Source: " << source << endl;
	if (prevChoice != choice)
		alSourceStop(source);
	alGetSourcei(source, AL_SOURCE_STATE, &source_state);
	checkError();
	if (source_state == AL_PLAYING)
	{

		//cout << "PLAY AUDIO SOURCE:  " << source << " IS PLAYING" << endl;
		return;
	}
	else
	{
		alBufferData(toPlay.buffer, toPlay.format, (ALvoid*)toPlay.songBuf, (ALsizei)toPlay.dataSize, (ALsizei)toPlay.sampleRate);
		checkError();
		alSourcei(source, AL_SOURCE_RELATIVE, TRUE);
		
		alSourcei(source, AL_BUFFER, toPlay.buffer);
		checkError();
		alSourcePlay(source);		
	}


}
void Audio::cleanUpAudio(wavFile wav)
{
	/*Clean up*/
	alDeleteSources(1, &wav.source);
	alDeleteBuffers(1, &wav.buffer);
	Device = alcGetContextsDevice(context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(Device);
}
/*Modify volume, pitch distance*/
ALuint Audio::sourceSetup(ALuint source, float vol, glm::vec3 pos, bool backSound)
{
	ALfloat x, y, z;
	alGetListener3f(AL_POSITION, &x, &y, &z);
	vec3 lisPos = vec3((float)x, (float)y, (float)z);
	//float dist = glm::length(lisPos - pos);
	//alSourcef(source, AL_REFERENCE_DISTANCE, 1.0f);
	//alSourcef(source, AL_ROLLOFF_FACTOR, 1.0f);
	//dist = fmax(dist, AL_REFERENCE_DISTANCE);
//	dist = fmin(dist, AL_MAX_DISTANCE);

	/*Either this or maybe i need to set the reference_dist to where the listener currenty is all the time*/
	//ALfloat gain = AL_REFERENCE_DISTANCE / (AL_REFERENCE_DISTANCE + AL_ROLLOFF_FACTOR * (dist - AL_REFERENCE_DISTANCE));
	//cout << "Gain: " << gain << endl;
	if (!backSound)
		alDistanceModel(AL_INVERSE_DISTANCE);
	else
		alDistanceModel(AL_NONE);

	alSourcef(source, AL_PITCH, 1.0f);
	checkError();

	//alSourcef(source, AL_GAIN, gain);
	//checkError();

	alSource3f(source, AL_POSITION, pos.x, pos.y, pos.z);
	checkError();

	alSource3f(source, AL_VELOCITY, 0, 0, 0);
	checkError();

	alSourcei(source, AL_LOOPING, AL_FALSE);
	checkError();

	return source;
}

ALenum Audio::formatWav(wavFile wav)
{
	ALenum format;
	if (wav.bitsPerSample == 8)
	{
		if (wav.channels == 1)
			format = AL_FORMAT_MONO8;
		else if (wav.channels == 2)
			format = AL_FORMAT_STEREO8;
	}
	if (wav.bitsPerSample == 16)
	{
		if (wav.channels == 1)
			format = AL_FORMAT_MONO16;
		else if (wav.channels == 2)
			format = AL_FORMAT_STEREO16;
	}
	return format;

}
/*Initializes the wav file to be read*/
wavFile Audio::openWavFile(string fileName, wavFile toPlay)
{
	wavFile wav = toPlay;

	char type[4];
	DWORD size, chunkSize;
	short formatType, channels;
	DWORD sampleRate, avgBytesPerSec;
	short bytesPerSample, bitsPerSample;
	DWORD dataSize;
	unsigned char* buf;



	FILE *fp = nullptr;
	fopen_s(&fp, fileName.c_str(), "rb");


	fread(type, sizeof(char), 4, fp);
	if (type[0] != 'R' || type[1] != 'I' || type[2] != 'F' || type[3] != 'F')
	{
		cout << "RIFF PRINT OUT: " << type[0] << type[1] << type[2] << type[3] << endl;
		cout << "No RIFF" << endl;
	}

	fread(&size, sizeof(DWORD), 1, fp);

	fread(type, sizeof(char), 4, fp);
	if (type[0] != 'W' || type[1] != 'A' || type[2] != 'V' || type[3] != 'E')
	{
		cout << "WAVE PRINT OUT: " << type[0] << type[1] << type[2] << type[3] << endl;
		cout << "Not a WAVE" << endl;
	}

	fread(type, sizeof(char), 4, fp);

	if (type[0] != 'f' || type[1] != 'm' || type[2] != 't' || type[3] != ' ')
	{
		cout << "FMT PRINT OUT: " << type[0] << type[1] << type[2] << type[3] << endl;
		cout << "No fmt " << endl;
	}

	fread(&chunkSize, sizeof(DWORD), 1, fp);
	fread(&formatType, sizeof(short), 1, fp);
	fread(&channels, sizeof(short), 1, fp);
	fread(&sampleRate, sizeof(DWORD), 1, fp);
	fread(&avgBytesPerSec, sizeof(DWORD), 1, fp);
	fread(&bytesPerSample, sizeof(short), 1, fp);
	fread(&bitsPerSample, sizeof(short), 1, fp);

	fread(type, sizeof(char), 4, fp);
	if (type[0] != 'd' || type[1] != 'a' || type[2] != 't' || type[3] != 'a')
	{
		cout << "DATA PRINT OUT: " << type[0] << type[1] << type[2] << type[3] << endl;
		cout << "No DATA" << endl;
	}

	fread(&dataSize, sizeof(DWORD), 1, fp);

	buf = new unsigned char[dataSize];
	fread(buf, sizeof(BYTE), dataSize, fp);

	wav.size = size;
	wav.chunkSize = chunkSize;
	wav.formatType = formatType;
	wav.channels = channels;
	wav.sampleRate = sampleRate;
	wav.avgBytesPerSec = avgBytesPerSec;
	wav.bytesPerSample = bytesPerSample;
	wav.bitsPerSample = bitsPerSample;
	wav.dataSize = dataSize;
	wav.songBuf = buf;
	return wav;

}
/*openAL apparently needs us to check for errors constantly, this just makes it easier*/
void Audio::checkError()
{
	ALCenum error;
	error = alGetError();

	switch (error)
	{
	case AL_INVALID_NAME:
	{
		cout << "INVALID NAME" << endl;
		break;
	}
	case AL_INVALID_ENUM:
	{
		cout << "INVALID ENUM" << endl;
		break;
	}
	case AL_INVALID_VALUE:
	{
		cout << "INVALID VALUE" << endl;
		break;
	}
	case AL_INVALID_OPERATION:
	{
		cout << "INVALID OPERATION" << endl;
		break;
	}
	case AL_OUT_OF_MEMORY:
	{
		cout << "OUT OF MEMORY" << endl;
		break;
	}

	default:
		break;
	}


}
/*Probably not need just shows what audio devices are available*/
void Audio::list_audio_devices(const ALCchar *devices)
{
	const ALCchar *device = devices, *next = devices + 1;
	size_t len = 0;

	fprintf(stdout, "Devices list:\n");
	fprintf(stdout, "----------\n");
	while (device && *device != '\0' && next && *next != '\0') {
		fprintf(stdout, "%s\n", device);
		len = strlen(device);
		device += (len + 1);
		next += (len + 2);
	}
	fprintf(stdout, "----------\n");
}
void Audio::playSounds(Entity &entity)
{
	//ALuint source;
	vector<int> choice;
	vector<ALuint> sources;
	vector<int> prevChoices;
	vector<vec3> soundPos;
	Events::Sound e{sources, choice, prevChoices, soundPos};
	
	entity.BroadcastEvent(e);

	for (int i = 0; i < e.sources.size(); i++)
	{
		e.sources[i] = sourceSetup(e.sources[i], 0.01f, soundPos[i], false);
		//alSourcei(e.sources[i], AL_SOURCE_RELATIVE, AL_TRUE);
		//alSourcef(e.sources[i], AL_ROLLOFF_FACTOR, 0.0f);
	//	alSourcePause(e.sources[i]); 
		 
		playAudio(e.choice[i], e.sources[i], prevChoices[i]);	
	}

}
