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

#include <stdlib.h>
#include <math.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "powerupfactory.h" // class's header file
#include "renderer.h"
#include "powerup.h"
#include "collisionmask.h"
#include "imageloader.h"

using namespace Balder;

// class constructor
PowerUpFactory::PowerUpFactory(Renderer* render)
{
    sprite = ImageLoader::LoadImage("powerups.png");
    if (sprite == 0)
    {
        throw "could not load PowerUp sprite!";
    }
    render->RegisterSpriteType("powerup", sprite);
}

// class destructor
PowerUpFactory::~PowerUpFactory()
{
	ImageLoader::FreeImage("powerups.png");
}

void PowerUpFactory::SetCollisionMask ( CollisionMask* c )
{
	collision_mask = c;
}

/**
  * creates a PowerUp */
PowerUp* PowerUpFactory::CreatePowerUp(GameManager* gm, int seed)
{
    srand(seed);
    int x = rand()%collision_mask->GetWidth();
    int y = rand()%collision_mask->GetHeight();
    while (collision_mask->TestPixel(x, y)) {
        x = rand()%collision_mask->GetWidth();
        y = rand()%collision_mask->GetHeight();
    }
    int type = static_cast<int>(pow(2,rand()%NUM_POWERUP_TYPES));
    return new PowerUp(gm, x, y, 0.5-rand()%10/10.0, 0.5-rand()%10/10.0, type);
}
