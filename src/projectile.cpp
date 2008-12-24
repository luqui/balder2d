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

#include "projectile.h"
#include "gamemanager.h"
#include "configmanager.h"

using namespace Balder;

const int Projectile::PROJECTILEWIDTH = ConfigManager::GetIntValue("Projectile", "radius");
const int Projectile::PROJECTILEHEIGHT = ConfigManager::GetIntValue("Projectile", "radius");
const int Projectile::PROJECTILELIFETIME = ConfigManager::GetIntValue("Projectile", "lifetime");
const int Projectile::MAX_FRAME = ConfigManager::GetIntValue("Projectile", "frames");
const int Projectile::EXPLODE_FRAMES = ConfigManager::GetIntValue("Projectile", "explodeframes");
const int Projectile::EXPLODE_RADIUS = ConfigManager::GetIntValue("Projectile", "exploderadius");

Projectile::Projectile(GameManager* mg, double x_pos, double y_pos,
	double x_vel, double y_vel, player_id pid)
    :Entity(mg, x_pos, y_pos, x_vel, y_vel, PROJECTILEWIDTH, PROJECTILEHEIGHT),
    time_remaining(PROJECTILELIFETIME), frame_number(0), owner(pid), is_exploding(false)
{
	bounding_box.x = 0;
	bounding_box.y = 0;
    bounding_box.w = PROJECTILEWIDTH;
    bounding_box.h = PROJECTILEHEIGHT;
}

void Projectile::Move()
{
	Entity::Move();
	--time_remaining;
	if(0 == time_remaining) {
		if (is_exploding){is_dead = true;}
		else {Explode();}
	}
	++frame_number;
	if (MAX_FRAME == frame_number){
		frame_number = 0;
	}
}

bool Projectile::Collide(Entity* e)
{
    int x = e->GetX() - GetX();
    int y = e->GetY() - GetY();
    if (x*x + y*y > (PROJECTILEWIDTH/2 + e->GetWidth()/2)
    				*(PROJECTILEHEIGHT/2 + e->GetHeight()/2)){
    	return false;
    }
    // there is a collision, but if we're already blowing up just return true;
    if (is_exploding) {return true;}
    // don't anialate friendly projectiles for the first few frames
    if (0 == e->GetType().compare("projectile")
        && (time_remaining > PROJECTILELIFETIME - 30)
        && owner == dynamic_cast<Projectile*>(e)->GetOwner())
    {
    	return false;
    }
    // we hit something that wasn't a wall, blow up!
	Explode();
    return true;
}

void Projectile::Explode()
{
	is_exploding = true;
    time_remaining = EXPLODE_FRAMES;
    frame_number = 0;
    // also slow down by 1/2
    x_velocity /= 2;
    y_velocity /= 2;
    bounding_box.w = EXPLODE_RADIUS;
    bounding_box.h = EXPLODE_RADIUS;
    bounding_box.y = PROJECTILEHEIGHT;
}

void Projectile::Die()
{
	game_manager->RemoveEntity(this);
}

SDL_Rect* Projectile::GetFrame (  )
{
	if (is_exploding) {bounding_box.x = EXPLODE_RADIUS*frame_number;}
	else {bounding_box.x = PROJECTILEWIDTH*frame_number;}
    return &bounding_box;
}
