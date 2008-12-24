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

#include <guichan/rectangle.hpp>
#include <guichan/widgets/icon.hpp>
#include <guichan/sdl/sdlgraphics.hpp>
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include "menu/widgets/sampleprobe.h"
#include "probe.h"
#include "imageloader.h"
#include "log.h"
using namespace Balder;

SampleProbe::SampleProbe(): probeFrame(0), probeColor(0)
{
    // Loads sprite for sample probe.
    probeSpriteImage = ImageLoader::LoadGcnImage("probe.png");
    if (probeSpriteImage == 0){
        throw "could not load Probe sprite!";
    }
    setHeight(PROBEWIDTH);
    setWidth(PROBEHEIGHT);
    frameTimer = SDL_GetTicks();
}

void SampleProbe::draw(gcn::Graphics* graphics)
{
    // looks like we still have to draw directly to the screen here...
    // no doubt this will choke if the obect is not an SDLGraphics object
    SDL_Surface *screen = dynamic_cast<gcn::SDLGraphics*>(graphics)->getTarget();
    int x,y;
    gcn::Widget::getAbsolutePosition(x,y);
    filledCircleColor(screen, x+PROBEWIDTH/2, y+PROBEHEIGHT/2, (PROBEWIDTH/2)-1, probeColor);
    if (SDL_GetTicks() - frameTimer > SAMPLEPROBE_FRAME_INTERVAL) {
        frameTimer = SDL_GetTicks();
        ++probeFrame;
        if (probeFrame > MAXFRAME) probeFrame = 0;
    }
    graphics->drawImage(probeSpriteImage, probeFrame*PROBEWIDTH, 0, 0, 0, PROBEWIDTH, PROBEHEIGHT);

}

void SampleProbe::drawBorder(gcn::Graphics* graphics)
{
    gcn::Color faceColor = getBaseColor();
    gcn::Color highlightColor, shadowColor;
    int alpha = getBaseColor().a;
    int width = getWidth() + getFrameSize() * 2 - 1;
    int height = getHeight() + getFrameSize() * 2 - 1;
    highlightColor = faceColor + 0x303030;
    highlightColor.a = alpha;
    shadowColor = faceColor - 0x303030;
    shadowColor.a = alpha;

    unsigned int i;
    for (i = 0; i < getFrameSize(); ++i)
    {
        graphics->setColor(shadowColor);
        graphics->drawLine(i,i, width - i, i);
        graphics->drawLine(i,i + 1, i, height - i - 1);
        graphics->setColor(highlightColor);
        graphics->drawLine(width - i,i + 1, width - i, height - i);
        graphics->drawLine(i,height - i, width - i - 1, height - i);
    }
}
