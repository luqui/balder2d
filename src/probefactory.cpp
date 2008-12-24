/***************************************************************************
 *   Copyright (C) 2004 by Bjorn Hansen                                    *
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

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "probefactory.h" // class's header file
#include "renderer.h"
#include "soundmanager.h"
#include "probe.h"
#include "imageloader.h"

using namespace Balder;

// class constructor
ProbeFactory::ProbeFactory(Renderer* render, SoundManager* sound)
{
    sprite = ImageLoader::LoadImage("probe.png");
    if (sprite == 0)
    {
        throw "could not load Probe sprite!";
    }
    render->RegisterSpriteType("probe", sprite);
    // now load the sounds
    sound->LoadSoundType("fire", "sounds/fire.ogg");
    sound->LoadSoundType("probe bounce", "sounds/probe_bounce.ogg");
    sound->LoadSoundType("probe collide", "sounds/probe_collide.ogg");
    sound->LoadSoundType("probe hit", "sounds/probe_hit.ogg");
    sound->LoadSoundType("probe destroyed", "sounds/probe_destroyed.ogg");
    sound->LoadSoundType("probe stick", "sounds/probe_stick.ogg");
    sound->LoadSoundType("probe push", "sounds/probe_push.ogg");
    sound->LoadSoundType("shields up", "sounds/probe_shields.ogg");
}

// class destructor
ProbeFactory::~ProbeFactory()
{
	ImageLoader::FreeImage("probe.png");
}

/**
  * creates a Probe */
Probe* ProbeFactory::CreateProbe(GameManager* gm, double x_position, double y_position,
    double x_velocity, double y_velocity, bool stuck, probe_color color, player_id pid)
{
    return new Probe(gm, x_position, y_position, x_velocity, y_velocity, stuck, color, pid);
}
