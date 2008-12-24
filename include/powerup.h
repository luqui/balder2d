//
// C++ Interface: powerup
//
// Description:
//
//
// Author: Bjorn Hansen <Holomorph@users.sourceforge.net>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef BALDERPOWERUP_H
#define BALDERPOWERUP_H

#include "entity.h"

namespace Balder {
const int POWERUP_WIDTH = 16;
const int POWERUP_HEIGHT = 16;
//powerup effects
const int EFFECT_SPREADFIRE = 1;
const int EFFECT_SHIELD = 2;
const int EFFECT_RECHARGE_FAST = 4;
const int NUM_POWERUP_TYPES = 3; // number of types of powerups

/**
Represents various powerups that players can collect

@author Bjorn Hansen
*/
class PowerUp : public Entity
{
public:
    PowerUp(GameManager* , double x_position, double y_position,
        double x_velocity, double y_velocity, int effect=1);
    ~PowerUp();
    const std::string GetType() const { return "powerup";}
    SDL_Rect* GetFrame (  ) {return &bounding_box;}
    int getEffect() {return effect;}
    bool Collide(Entity*);
private:
	int effect;
};

}

#endif
