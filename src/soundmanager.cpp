/***************************************************************************
 *   Copyright (C) 2007 by Bjorn Hansen                                    *
 *   holomorph@users.sourceforge.net                                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "soundmanager.h"
#include "filepathmanager.h"

using namespace Balder;

SoundManager::SoundManager()
{
    Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,512);
    PlayMusic("menu");
    double volume = ConfigManager::GetDoubleValue("Sound", "fxvolume");
    Mix_Volume(-1, (int)(MIX_MAX_VOLUME * volume));
}
SoundManager::~SoundManager()
{
    StopMusic();
    Mix_CloseAudio();
}

bool SoundManager::LoadSoundType(const char* name, const char* filename)
{
    if(sound_registry.count(name)){return true;} // there is already a sound with that name loaded
    std::string fullpath = FilePathManager::GetFullPath(filename);
    if (fullpath.empty()) {return false;}
	Mix_Chunk *sample = Mix_LoadWAV(fullpath.c_str());
    if (0 == sample) { return false;}
	sound_registry[name] = sample;
	return true;
}

void SoundManager::PlaySound(const char *name)
{
    if (ConfigManager::GetIntValue("Sound", "sound")) {
    	Mix_Chunk *sample = sound_registry[name];
	    if (sample){
		    Mix_PlayChannel(-1, sample, 0);
        }
	}
}

void SoundManager::PlayMusic(std::string name)
{
    StopMusic();

    if (name != "NONE" || name != "None" || name != "none") {
        std::string music_file = "music/";
        music_file += name;
        music_file += ".ogg";
        if (ConfigManager::GetIntValue("Sound", "music")) {
            std::string fullpath = FilePathManager::GetFullPath(music_file.c_str());
            music = Mix_LoadMUS(fullpath.c_str());
            if (music){
    	        Mix_PlayMusic(music,1);
                double volume = ConfigManager::GetDoubleValue("Sound", "musicvolume");
    	        Mix_VolumeMusic(int(SDL_MIX_MAXVOLUME*volume));
            }
        }
    }
}

void SoundManager::StopMusic()
{
    if (Mix_PlayingMusic()) {
    	if (music){
	        Mix_FreeMusic(music);
	        music = 0;
	    }
    }
}
