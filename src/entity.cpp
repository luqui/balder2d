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

#include "log.h"
#include "entity.h"
#include "gamemanager.h"
#include "configmanager.h"
#include "collisionmask.h"
#include <math.h>

using namespace Balder;

const int Entity::MAXVELOCITY = ConfigManager::GetIntValue("Entity", "maxvelocity");

Entity::Entity ( GameManager* gm,
    double xp, double yp, double xv, double yv, int w, int h )
    : game_manager(gm), x_position(xp),y_position(yp),
     x_velocity(xv), y_velocity(yv), width(w), height(h), is_dead(false)
{
	game_manager->RegisterEntity(this);
}

SDL_Rect* Entity::GetFrame (  )
{
    return 0;
}

void Entity::DrawOnto(SDL_Surface* destination)
{
    SDL_Surface* s = Renderer::sprite_registry[GetType()];
    if (!s) throw "no sprite found for entity!";
    SDL_Rect* sourceRect = GetFrame();
    SDL_Rect target = {GetX()-sourceRect->w/2, GetY()-sourceRect->h/2,
                         sourceRect->w, sourceRect->h};
    SDL_BlitSurface( s, sourceRect, destination, &target);
}

const std::string Entity::GetType() const
{
    return "none";
}

int Entity::GetWidth (  )
{
    return width;
}

int Entity::GetHeight (  )
{
    return height;
}

void Entity::Move (  )
{
	// if the entity is not moving, don't bother . .
//	if ( (0 == x_velocity) && (0 == y_velocity)) {return;}
	// make sure the entity is not moving too fast
	if (x_velocity > MAXVELOCITY){ x_velocity = MAXVELOCITY;}
	if (y_velocity > MAXVELOCITY){ y_velocity = MAXVELOCITY;}
    x_position += x_velocity;
    y_position += y_velocity;
    // if we're off the screen, wrap
    if (x_position > game_manager->GetWidth()) x_position = 0;
    if (x_position < 0) x_position = game_manager->GetWidth();
    if (y_position > game_manager->GetHeight()) y_position = 0;
    if (y_position < 0) y_position = game_manager->GetHeight();
    // now test for collisions with walls
    int collision_count = 0;
	int x_ave = 0;
    int y_ave = 0;
	int x_offset, y_offset;

    int x, y, radius, r2;
    radius = width/2;

    r2 = radius * radius;
    if (collision_mask->TestPixel(GetX(), GetY() + radius)){y_ave +=radius;++collision_count;}
    if (collision_mask->TestPixel(GetX(), GetY() - radius)){y_ave -=radius;++collision_count;}
    if (collision_mask->TestPixel(GetX() + radius, GetY())){x_ave +=radius;++collision_count;}
    if (collision_mask->TestPixel(GetX() - radius, GetY())){x_ave -=radius;++collision_count;}
    x = 1;
    y = (int) (sqrt(r2 - 1) + 0.5);
    while (x < y) {
        if (collision_mask->TestPixel(GetX() + x, GetY() + y))
        {x_ave +=x; y_ave+=y;++collision_count;}
        if (collision_mask->TestPixel(GetX() + x, GetY() - y))
        {x_ave +=x; y_ave-=y;++collision_count;}
        if (collision_mask->TestPixel(GetX() - x, GetY() + y))
        {x_ave -=x; y_ave+=y;++collision_count;}
        if (collision_mask->TestPixel(GetX() - x, GetY() - y))
        {x_ave -=x; y_ave-=y;++collision_count;}
        if (collision_mask->TestPixel(GetX() + y, GetY() + x))
        {x_ave +=y; y_ave+=x;++collision_count;}
        if (collision_mask->TestPixel(GetX() + y, GetY() - x))
        {x_ave +=y; y_ave-=x;++collision_count;}
        if (collision_mask->TestPixel(GetX() - y, GetY() + x))
        {x_ave -=y; y_ave+=x;++collision_count;}
        if (collision_mask->TestPixel(GetX() - y, GetY() - x))
        {x_ave -=y; y_ave-=x;++collision_count;}
        ++x;
        y = (int) (sqrt(r2 - x*x) + 0.5);
    }
    if (x == y) {
        if (collision_mask->TestPixel(GetX() + x, GetY() + y))
        {x_ave +=x; y_ave+=y;++collision_count;}
        if (collision_mask->TestPixel(GetX() + x, GetY() - y))
        {x_ave +=x; y_ave-=y;++collision_count;}
        if (collision_mask->TestPixel(GetX() - x, GetY() + y))
        {x_ave -=x; y_ave+=y;++collision_count;}
        if (collision_mask->TestPixel(GetX() - x, GetY() - y))
        {x_ave -=x; y_ave-=y;++collision_count;}
    }
    if (collision_count){
        // average the pixel offset coordinates
        x_offset = x_ave/collision_count;
        y_offset = y_ave/collision_count;
        Bounce(x_offset, y_offset);
    }
}

void Entity::Die()
{
	game_manager->RemoveEntity(this);
}

bool Entity::Collide ( Entity*  )
{
	return false;
}

void Entity::SetCollisionMask ( CollisionMask* c )
{
	collision_mask = c;
}

void Entity::Bounce ( double x, double y )
{
	// first normalize the normal vector :p
	double norm = sqrt(x*x+y*y);
	x = x/norm;
	y = y/norm;
	// dot the normal vector with the velocity vector (actually what we have
	// here is the negative of the normal to the surface.
	double projection = x*x_velocity + y*y_velocity;
	// make sure the vectors are within 90 degrees of each other, otherwise the
	// entity is already moving away from the wall
	if (projection < 0) {return;}
	// ok calculate the final velocity vector
	x_velocity = x_velocity - 2*projection * x;
	y_velocity = y_velocity - 2*projection * y;
}
