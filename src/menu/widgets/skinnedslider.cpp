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

#include <guichan/exception.hpp>
#include <guichan/sdl/sdlgraphics.hpp>
#include <guichan/font.hpp>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "menu/widgets/skinnedslider.h"
#include "imageloader.h"
using namespace Balder;
using namespace gcn;

SkinnedSlider::SkinnedSlider(std::string sFN/*strip file name*/, unsigned int sSL/*strip vertical location in pixels*/, double scaleStart/*scale starting point*/, double scaleEnd/*scale ending point*/, int sML/*marker length in pixels*/):
        skinFileName(sFN), skinStripLocation(sSL), widgetState(widgetNotFocused)
{
    //setFrameSize(0); //doesn't exist anymore in guichan 8.1
    setScaleStart(scaleStart);
    setScaleEnd(scaleEnd);
    setOrientation(HORIZONTAL);
    setMarkerLength(sML);

    // Load Skin Frame Strip from File.
    skinFrameStrip = ImageLoader::LoadImage(skinFileName.c_str());
    if (skinFrameStrip == 0) {
        throw "could not load button skin!";
    }
}

SkinnedSlider::~SkinnedSlider()
{
    if (skinFrameStrip != 0) {
        SDL_FreeSurface(skinFrameStrip);
    }
}

void SkinnedSlider::draw(gcn::Graphics* graphics)
{
    Color highlight = MenuColors::HIGHLIGHT;
    Color shadow = MenuColors::SHADOW;
    if (isFocused())
    {
      Color bgColor = MenuColors::FOCUSED;
      bgColor.a = MenuColors::bgAlpha;
      graphics->setColor(bgColor);
      graphics->fillRectangle(Rectangle(0, 0, getWidth(), getHeight()));
    }
    // draw the border
    for (int i=0; i<5 ;++i)
    {
      shadow.a -= i*0x10;
      graphics->setColor(shadow);
      graphics->drawLine(i,i,getWidth()-i,i);
      graphics->drawLine(i,i,i,getHeight()-i);
    }
    for (int i=0; i<5 ;++i)
    {
      highlight.a -= i*0x10;
      graphics->setColor(highlight);
      graphics->drawLine(getWidth()-i,i,getWidth()-i,getHeight()-i);
      graphics->drawLine(i,getHeight()-i,getWidth()-i,getHeight()-i);
    }
    drawMarker(graphics);
}

void SkinnedSlider::drawBorder(gcn::Graphics* graphics)
{
    // Borders are not drawn for skinned buttons.
}

void SkinnedSlider::drawMarker(gcn::Graphics* graphics)
{
  int v = getMarkerPosition();
  // marker same color as the font -
  // this should probably be a const in balder2dtypes but so far this is the only place it's used.
  graphics->setColor(MenuColors::fontColor);
  graphics->fillRectangle(gcn::Rectangle(v + 1, 1, getMarkerLength() - 2, getHeight() - 2));
}
