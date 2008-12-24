/***************************************************************************
 *   Copyright (C) 2007 by Bjorn Hansen                                    *
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

#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include <SDL/SDL_image.h>
#include <guichan.hpp>
#include <guichan/sdl.hpp>
#include <map>

namespace Balder{

class ImageLoader{
public:
	static SDL_Surface* LoadImage(const char* filename);
	static void FreeImage(const char* filename);
	static gcn::Image* LoadGcnImage(const char* filename);
	static gcn::ImageFont* LoadGcnImageFont(const char* filename);
private:
    static std::map<std::string,SDL_Surface*> SDLImages;
    static std::map<std::string,gcn::Image*> gcnImages;
};
}
#endif // LOG_H

