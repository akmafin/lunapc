#include "lunapc.h"

void SoundInit(maindata *lunadata) {
	int init = Mix_Init(MIX_INIT_OGG);

	if ((init&MIX_INIT_OGG) != MIX_INIT_OGG)
	{
		//std::cout << "Sound init failed: " << Mix_GetError();
		//sndena = false;
	}
	else
	{
		if ((init = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)) == 0)
		{
			Mix_AllocateChannels(AUDIO_CHANNELS); // Amount of mixed sound channels
			Mix_ReserveChannels(AUDIO_RESERVED_CHS); // How many channels to reserve for 'play on first free channel' (channel -1)
			lunadata->sound.mixchunk[SOUND_SHOOT] = Mix_LoadWAV("assets/hallow-shoot.wav");
			lunadata->sound.mixchunk[SOUND_EXPLODE] = Mix_LoadWAV("assets/hallow-explode.wav");
			lunadata->sound.mixchunk[SOUND_BONUS] = Mix_LoadWAV("assets/hallow-bonus.wav");
			
			lunadata->sound.musicgame = Mix_LoadMUS("assets/hallow-music.ogg");
			lunadata->sound.musicgameover = Mix_LoadMUS("assets/hallow-gameover.wav");
			//std::cout << "MUS" << music << Mix_GetError() << std::flush;
			Mix_VolumeMusic(MIX_MAX_VOLUME * 0.9);
			//sndena = true;
		}
		else
		{
			//std::cout << "Unable to init sound source: " << Mix_GetError();
			//sndena = false;
		}
	}
}

void SoundGetNextChannel(maindata *lunadata) {
}

void SoundSfxShoot(maindata *lunadata) {
	Mix_PlayChannel(lunadata->sound.sndchan[SOUND_SHOOT], lunadata->sound.mixchunk[SOUND_SHOOT], 0);
}

void SoundSfxExplode(maindata *lunadata) {
	Mix_PlayChannel(lunadata->sound.sndchan[SOUND_EXPLODE], lunadata->sound.mixchunk[SOUND_EXPLODE], 0);
}

void SoundSfxBonus(maindata *lunadata) {
	Mix_PlayChannel(lunadata->sound.sndchan[SOUND_BONUS], lunadata->sound.mixchunk[SOUND_BONUS], 0);
}
