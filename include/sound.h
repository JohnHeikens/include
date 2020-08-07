#pragma once
#include "GlobalFunctions.h"
//source:
//http://www.rastertek.com/dx10tut14.html

/////////////
// LINKING //
/////////////
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")


//////////////
// INCLUDES //
//////////////
#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>
//must be WAV-sound.
inline void startsound(LPCWSTR filename)
{
	PlaySound(filename, NULL, SND_FILENAME | SND_ASYNC);
}
inline void stopsound()
{
	PlaySound(NULL, 0, 0);
}

///////////////////////////////////////////////////////////////////////////////
// Class name: SoundClass
///////////////////////////////////////////////////////////////////////////////
class SoundClass
{
private:

	//The WaveHeaderType structure used here is for the .wav file format. When loading in .wav files I first read in the header to determine the required information for loading in the .wav audio data. If you are using a different format you will want to replace this header with the one required for your audio format.

	struct WaveHeaderType
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};

public:
	SoundClass();
	SoundClass(const SoundClass&);
	~SoundClass();
	bool Initialize(HWND hwnd, char* soundpath);
	//Initializeand Shutdown will handle everything needed for this tutorial.The Initialize function will initialize DirectSoundand load in the.wav audio fileand then play it once.Shutdown will release the.wav fileand shutdown DirectSound.

	void Shutdown();
	bool PlayWaveFile();

private:
	bool InitializeDirectSound(HWND);
	void ShutdownDirectSound();

	bool LoadWaveFile(char*, IDirectSoundBuffer8**);
	void ShutdownWaveFile(IDirectSoundBuffer8**);


private:
	IDirectSound8* m_DirectSound;
	IDirectSoundBuffer* m_primaryBuffer;
	//Note that I only have one secondary buffer as this tutorial only loads in one sound.

		IDirectSoundBuffer8* m_secondaryBuffer1;
};
