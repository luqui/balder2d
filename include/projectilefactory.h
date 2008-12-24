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

#ifndef PROJECTILEFACTORY_H
#define PROJECTILEFACTORY_H

#include "balder2dtypes.h"

namespace Balder{

class Renderer;
class Projectile;
class GameManager;

/**
 * A factory class for Projectiles which takes care of loading the Projectile
 * sprite and registering it with the Renderer.
 */
class ProjectileFactory{
	public:
		// class constructor
		ProjectileFactory(Renderer* render);
		// class destructor
		~ProjectileFactory();
		/// create a Projectile
        Projectile* CreateProjectile(GameManager* , double x_position, double y_position,
            double x_velocity, double y_velocity, player_id);
	private:
		SDL_Surface* sprite;

};

}
#endif // PROJECTILEFACTORY_H


