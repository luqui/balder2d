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

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "collisionmask.h"
#include "imageloader.h"
#include <string>

using namespace Balder;

CollisionMask::CollisionMask(const char* mapname)
{
	std::string filename("maps/");
	filename += mapname;
	filename += "/";
	filename += "geometry.png";
	SDL_Surface *field = ImageLoader::LoadImage(filename.c_str());
	if (!field) throw "invalid surface used to create collision mask";
	width = field->w;
	height = field->h;
	// allocate memory for the mask
	mask = new bool[width * height];

	int bpp = field->format->BytesPerPixel;
	for (int x = 0; x < width; x++)
	{
        for (int y = 0; y < height; y++)
        {
            /* Here p is the address to the pixel we want to retrieve */
            Uint8 *p = (Uint8 *)field->pixels + y * field->pitch + x * bpp;
            // if it is any color but black, count it as a wall
            if(*p > 0) mask[y*width + x] = true;
            else mask[y*width + x] = false;
        }
    }
    ImageLoader::FreeImage(filename.c_str());
}

CollisionMask::~CollisionMask()
{
	if (mask){
		delete mask;
		mask = 0;
	}
}
bool CollisionMask::TestPixel ( int x, int y )
{
    // first, make sure the point is within the collision field
    if (x > width-1 || x < 0 || y > height-1 || y < 0) return false;
    return mask[y*width + x];
}
