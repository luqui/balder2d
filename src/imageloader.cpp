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

#include "imageloader.h"
#include "log.h"
#include "filepathmanager.h"
#include <string>

using namespace Balder;

std::map<std::string,SDL_Surface*> ImageLoader::SDLImages;
std::map<std::string,gcn::Image*> ImageLoader::gcnImages;

SDL_Surface* ImageLoader::LoadImage(const char* filename)
{
    if (!SDLImages.count(filename))
    {
        SDLImages[filename] = IMG_Load(FilePathManager::GetFullPath(filename).c_str());
    }
    return SDLImages[filename];
}

void ImageLoader::FreeImage(const char* filename)
{
    if (SDLImages.count(filename))
    {
        SDL_FreeSurface(SDLImages[filename]);
        SDLImages.erase(filename);
    }
}

gcn::Image* ImageLoader::LoadGcnImage(const char* filename)
{
    if (!gcnImages.count(filename))
    {
        gcnImages[filename] = gcn::Image::load(FilePathManager::GetFullPath(filename).c_str());
    }
    return gcnImages[filename];
}

gcn::ImageFont* ImageLoader::LoadGcnImageFont(const char* filename)
{
    return new gcn::ImageFont(FilePathManager::GetFullPath(filename).c_str());
}
