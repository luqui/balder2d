#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <map>
#include <string>
#include <SDL/SDL_mixer.h>
#include "configmanager.h"

namespace Balder{

class SoundManager{
public:

	SoundManager();
	virtual ~SoundManager();
	bool LoadSoundType(const char* name, const char* file);
	void PlaySound(const char* name);
	void PlayMusic(std::string name);
	void StopMusic();
	const bool IsMusicPlaying() const {return Mix_PlayingMusic();}

private:
    bool musicPlaying;
	std::map<std::string, Mix_Chunk*> sound_registry;
	Mix_Music *music;
};
}
#endif // SOUNDMANAGER_H
