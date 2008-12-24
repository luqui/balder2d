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

#ifndef ENTITY_H
#define ENTITY_H

#include <SDL/SDL.h>
#include "balder2dtypes.h"
#include "renderer.h"

namespace Balder{

class GameManager;
class CollisionMask;

class Entity {
  private:
  	static const int MAXVELOCITY;
  public:
    Entity ( GameManager* , double x_position, double y_position,
        double x_velocity, double y_velocity, int width, int height );
	const int GetX() const {return static_cast<int>(x_position);}
	const int GetY() const {return static_cast<int>(y_position);}
    const double GetXPosition()const {return x_position;}
    const double GetYPosition()const {return y_position;}
    const double GetXVelocity()const {return x_velocity;}
    const double GetYVelocity()const {return y_velocity;}
    virtual const player_id GetOwner()const {return 0;}
    virtual void Move (  );
    virtual const bool IsDead()const {return is_dead;}
    virtual void Die(); // probably reset or destroy the enitity
    virtual bool Collide ( Entity*  );
    int GetWidth ();
    int GetHeight ();
    virtual SDL_Rect* GetFrame ();
    virtual void DrawOnto(SDL_Surface*);
    virtual const std::string GetType() const;
    void SetCollisionMask ( CollisionMask* );
  protected:
    GameManager *game_manager;
    CollisionMask *collision_mask;
    double x_position;
    double y_position;
    double x_velocity;
    double y_velocity;
    const int width;
    const int height;
    bool is_dead;
    SDL_Rect bounding_box;
    void Bounce ( double x, double y );
};
}
#endif

