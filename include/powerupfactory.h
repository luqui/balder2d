/***************************************************************************
 *   Copyright (C) 2005 by Bjorn Hansen                                    *
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

#ifndef POWERUPFACTORY_H
#define POWERUPFACTORY_H

#include "balder2dtypes.h"

namespace Balder{

class Renderer;
class PowerUp;
class GameManager;
class CollisionMask;

/**
 * A factory class for PowerUps which takes care of loading the PowerUp
 * sprite and registering it with the Renderer.
 */
class PowerUpFactory{
	public:
		// class constructor
		PowerUpFactory(Renderer* render);
		// class destructor
		~PowerUpFactory();
        void SetCollisionMask ( CollisionMask* );
		/// create a PowerUp
        PowerUp* CreatePowerUp(GameManager*, int seed);
	private:
		SDL_Surface* sprite;
        CollisionMask *collision_mask;
};

}
#endif // PROJECTILEFACTORY_H


