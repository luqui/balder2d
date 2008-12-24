//
// C++ Implementation: powerup
//
// Description:
//
//
// Author: Bjorn Hansen <Holomorph@users.sourceforge.net>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <math.h>
#include <SDL/SDL_image.h>
#include "powerup.h"

namespace Balder {

PowerUp::PowerUp(GameManager* gm, double x_position, double y_position,
        double x_velocity, double y_velocity, int effect)
 : Entity(gm, x_position, y_position,
 			x_velocity, y_velocity,
 			POWERUP_WIDTH, POWERUP_HEIGHT),
 effect(effect)
{
	bounding_box.x = Sint16(POWERUP_WIDTH * (log(effect)/log(2)));
	bounding_box.y = 0;
    bounding_box.w = POWERUP_WIDTH;
    bounding_box.h = POWERUP_HEIGHT;
}


PowerUp::~PowerUp()
{
}

bool PowerUp::Collide(Entity* e)
{
    int x = e->GetX() - GetX();
    int y = e->GetY() - GetY();
    if (x*x + y*y > (POWERUP_WIDTH/2 + e->GetWidth()/2)
    				*(POWERUP_HEIGHT/2 + e->GetHeight()/2)){
    	return false;
    }
    is_dead = true;
    return true;
}

}
