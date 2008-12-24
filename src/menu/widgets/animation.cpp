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
#include <guichan/sdl/sdlgraphics.hpp>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "menu/widgets/animation.h"
#include "imageloader.h"
using namespace Balder;

Animation::Animation(std::string aFileName, bool aHasBaseFrame,
    bool aLooped, bool aPlaying, unsigned int aStripLocation,
    unsigned int fWidth, unsigned int fHeight, unsigned int fBase,
    unsigned int fStart, unsigned int fQuantity, unsigned int fInterval)
{
    // Load Animation.
    animationFileName = aFileName;
    animationHasBaseFrame = aHasBaseFrame;
    animationLooped = aLooped;
    animationPlaying = aPlaying;
    animationStripLocation = aStripLocation;
    frameWidth = fWidth;
    frameHeight = fHeight;
    frameBase = fBase;
    frameStart = fStart;
    frameQuantity = fQuantity;
    frameInterval = fInterval;
    frameCurrent = 0;

    // Load Animation Frame Strip from File.
    animationFrameStrip = ImageLoader::LoadImage(animationFileName.c_str());
    if (animationFrameStrip == 0) {
        throw "could not load animation!";
    }

    // Set widget deminsions based on frame size.
    setWidth(Uint32(frameWidth));
    setHeight(Uint32(frameHeight));

    // Check to see if should start playing.
    if (aPlaying) {
        Restart();
    }
}

Animation::~Animation()
{
    if (animationFrameStrip != 0) {
        SDL_FreeSurface(animationFrameStrip);
    }
}

void Animation::Play()
{
    animationPlaying = true;
    frameTimer = SDL_GetTicks();
}

void Animation::Restart()
{
    frameCurrent = 0;
    Play();
}

void Animation::Animate()
{
    if (getAnimationPlaying()) {
        if (SDL_GetTicks() - frameTimer > frameInterval) {
            frameTimer = SDL_GetTicks();
            if (frameCurrent < (frameQuantity - 1)) {
                frameCurrent++;
            }
            else {
                if (getAnimationLooped()) {
                    Restart();
                }
                else {
                    Stop();
                }
            }
        }
    }
}

void Animation::draw(gcn::Graphics* graphics)
{
    // get screen that the menu is drawn on.
    SDL_Surface *screen = dynamic_cast<gcn::SDLGraphics*>(graphics)->getTarget();

    // get location of the widget on the menu screen.
    int x,y;
    gcn::Widget::getAbsolutePosition(x,y);

    // draw widget onto menu screen at widget location.
    SDL_Rect rectDest = {x, y, /*width is ignored*/0, /*height is ignored*/0};
    if (getAnimationHasBaseFrame()) {
        SDL_Rect rectSrc = {frameBase*frameWidth, animationStripLocation, frameWidth, frameHeight};
        SDL_BlitSurface(animationFrameStrip, &rectSrc, screen, &rectDest);
    }
    SDL_Rect rectSrc = {(frameStart+frameCurrent)*frameWidth, animationStripLocation, frameWidth, frameHeight};
    SDL_BlitSurface(animationFrameStrip, &rectSrc, screen, &rectDest);

    // Process next frame to display.
    Animate();
}
