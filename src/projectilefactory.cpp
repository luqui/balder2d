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
#include "projectilefactory.h" // class's header file
#include "renderer.h"
#include "projectile.h"
#include "imageloader.h"

using namespace Balder;

// class constructor
ProjectileFactory::ProjectileFactory(Renderer* render)
{
    sprite = ImageLoader::LoadImage("projectile.png");
    if (sprite == 0)
    {
        throw "could not load Projectile sprite!";
    }
    render->RegisterSpriteType("projectile", sprite);
}

// class destructor
ProjectileFactory::~ProjectileFactory()
{
    ImageLoader::FreeImage("projectile.png");
}

/**
  * creates a Projectile */
Projectile* ProjectileFactory::CreateProjectile(GameManager* gm, double x_position, double y_position,
    double x_velocity, double y_velocity, player_id pid)
{
    return new Projectile(gm, x_position, y_position, x_velocity, y_velocity, pid);
}
