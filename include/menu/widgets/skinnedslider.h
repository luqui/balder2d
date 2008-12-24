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

#ifndef BALDERSKINNEDSLIDER_H
#define BALDERSKINNEDSLIDER_H

#include <guichan/widgets/slider.hpp>
#include "../../balder2dtypes.h"

namespace Balder
{
    /**
     * An Icon for displaying an animated image.
     */
    class SkinnedSlider: public gcn::Slider
    {
    public:
        // Constructor and Destructor.
        SkinnedSlider(std::string, unsigned int, double, double, int);
        ~SkinnedSlider();

        // Inherited from gcn::Widget
        virtual void draw(gcn::Graphics* graphics);
        virtual void drawBorder(gcn::Graphics* graphics);
        virtual void drawMarker(gcn::Graphics* graphics);

    private:
        //Screen Variables.
        SDL_Surface* screen;
        int x;
        int y;

        // Skin Variables.
        SDL_Surface* skinFrameStrip;
        std::string skinFileName;
        unsigned int skinStripLocation;

        // Widget Variables.
        unsigned int widgetState;
    };
}

#endif  // BALDERSKINNEDSLIDER_H
