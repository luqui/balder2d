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

#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "entity.h"

namespace Balder{

class Projectile: public Entity {
  private:
	static const int PROJECTILEWIDTH;
	static const int PROJECTILEHEIGHT;
	static const int PROJECTILELIFETIME; // # of frames projectiles live for
	static const int MAX_FRAME;
  	static const int EXPLODE_FRAMES;
  	static const int EXPLODE_RADIUS;
    public:
    static const double SPEED;
    static const bool Add_Probe_Velocity;
    public:
    Projectile (GameManager* , double x_position, double y_position,
    	double x_velocity, double y_velocity, player_id);
    const std::string GetType() const { return "projectile";}
    void Move ( );
    bool Collide(Entity* e);
    void Explode();
    void Die();
    SDL_Rect* GetFrame (  );
    const player_id GetOwner(){return owner;}
    const bool IsExploding(){return is_exploding;}
  private:
    int time_remaining;
    int frame_number;
   	player_id owner;
   	bool is_exploding;
};
}
#endif
