/***************************************************************************
 *   Copyright (C) 2006 by Bjorn Hansen                                    *
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

#include <guichan/sdl/sdlgraphics.hpp>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>
#include "menu/widgets/samplemap.h"
#include "imageloader.h"

using namespace Balder;

SampleMap::SampleMap(std::string iniMapName, unsigned int X, unsigned int Y, unsigned int W, unsigned int H)
{
    setX(X);
    setY(Y);
    setWidth(W);
    setHeight(H);
    setSampleMap(iniMapName);
}

SampleMap::~SampleMap()
{
    if (map != 0) {
        SDL_FreeSurface(map);
    }
}

void SampleMap::setSampleMap(std::string name)
{
    // load map background.
    std::string mapFileName = "maps/" + name + "/background.png";
    SDL_Surface* tempSurf = ImageLoader::LoadImage(mapFileName.c_str());
    if (tempSurf == 0) {
        throw "could not load sample background!";
    }

    // get percentage to scale map size down to based off of widget size.
    double scaledX;
    double scaledY;
    double scaled;
    double src_pix_perc;
    double trg_pix_perc;

    src_pix_perc = tempSurf->w; // must convert int to double before preforming division to allow for floating point.
    src_pix_perc /= 100;
    trg_pix_perc = getWidth(); // must convert int to double before preforming division to allow for floating point.
    trg_pix_perc /= 100;

    scaledX = trg_pix_perc / src_pix_perc;

    src_pix_perc = tempSurf->h; // must convert int to double before preforming division to allow for floating point.
    src_pix_perc /= 100;
    trg_pix_perc = getHeight(); // must convert int to double before preforming division to allow for floating point.
    trg_pix_perc /= 100;

    scaledY = trg_pix_perc / src_pix_perc;

    if (scaledX < scaledY) scaled = scaledX;
    else scaled = scaledY;

    // scale down map to widget size.
        //zoom to rect.
    map = zoomSurface(tempSurf, scaled, scaled, /* anti-aliasing SMOOTHING_OFF=0 SMOOTHING_ON=1 */SMOOTHING_OFF);
}

void SampleMap::draw(gcn::Graphics* graphics)
{
    // get screen that the menu is drawn on.
    SDL_Surface *screen = dynamic_cast<gcn::SDLGraphics*>(graphics)->getTarget();

    // get location of the widget on the menu screen.
    int x,y;
    gcn::Widget::getAbsolutePosition(x,y);

    // draw widget onto menu screen at widget location.
    SDL_Rect rectSource = {0, 0, getWidth(), getHeight()};
    SDL_Rect rectDestination = {x+((getWidth()-map->w)/2), y+((getHeight()-map->h)/2), /*width is ignored*/0, /*height is ignored*/0};
    SDL_BlitSurface(map, &rectSource, screen, &rectDestination);
}

