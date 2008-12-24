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

#ifndef BALDERANIMATION_H
#define BALDERANIMATION_H

#include <guichan/image.hpp>
#include <guichan/widget.hpp>
#include "../../balder2dtypes.h"

namespace Balder
{
    /**
     * An Icon for displaying an animated image.
     */
    class Animation: public gcn::Widget
    {
    public:
        // Constructor and Destructor.
        Animation(std::string, bool, bool, bool, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
        ~Animation();

        // Controls whether or not there is a base frame in the animation background.
        void setAnimationHasBaseFrame(bool aHBF) { animationHasBaseFrame = aHBF; }
        const bool getAnimationHasBaseFrame() { return animationHasBaseFrame; }

        // Controls loop feature for playback.
        void setAnimationLooped(bool aL) { animationLooped = aL; }
        const bool getAnimationLooped() { return animationLooped; }

        // Playback control.
        const bool getAnimationPlaying() { return animationPlaying; }  // Chacks to see if animation is currently playing.
        void Play();  // Starts to play animation from frameCurrent.
        void Stop() { animationPlaying = false; }  // Stops playing animation at frameCurrent.
        void Restart();  // Resets frameCurrent to frameStart and then starts playing animation.

        // Animates next frame if playing.
        void Animate();

        // Inherited from gcn::Widget
        virtual void draw(gcn::Graphics* graphics);

    private:
        // Animation Variables.
        SDL_Surface* animationFrameStrip;
        std::string animationFileName;
        bool animationHasBaseFrame;
        bool animationLooped;
        bool animationPlaying;
        unsigned int animationStripLocation;

        // Frame Variables.
        unsigned int frameWidth;
        unsigned int frameHeight;
        unsigned int frameBase;
        unsigned int frameStart;
        unsigned int frameQuantity;
        unsigned int frameInterval;
        unsigned int frameCurrent;
        Uint32 frameTimer;
    };
}

#endif  // BALDERANIMATION_H
