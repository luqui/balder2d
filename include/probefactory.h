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

#ifndef PROBEFACTORY_H
#define PROBEFACTORY_H

#include "balder2dtypes.h"

namespace Balder{

class Renderer;
class Probe;
class GameManager;
class SoundManager;

/**
 * A factory class for Probes which takes care of loading the probe
 * sprite and registering it with the Renderer.
 */
class ProbeFactory{
	public:
		// class constructor
		ProbeFactory(Renderer* render, SoundManager *sound);
		// class destructor
		~ProbeFactory();
		/// create a probe
        Probe* CreateProbe(GameManager* , double x_position, double y_position,
            double x_velocity, double y_velocity, bool stuck, probe_color, player_id pid);
	private:
		SDL_Surface* sprite;

};
}
#endif // PROBEFACTORY_H


