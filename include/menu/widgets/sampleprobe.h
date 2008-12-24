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

#ifndef BALDERSAMPLEPROBE_H
#define BALDERSAMPLEPROBE_H

#include <guichan/image.hpp>
#include <guichan/widget.hpp>
#include "../../balder2dtypes.h"

namespace Balder
{
// This is the interval (in milliseconds) in between frames for the sample probe
const int SAMPLEPROBE_FRAME_INTERVAL = 50;

    /**
     * An Icon for displaying an animated sample probe.
     */
    class SampleProbe: public gcn::Widget
    {
    public:
        /**
         * Constructor.
         *
         * @param image an Image to display.
         */
        SampleProbe();


        // Inherited from Widget

        virtual void draw(gcn::Graphics* graphics);

        virtual void drawBorder(gcn::Graphics* graphics);

        void setProbeColor(probe_color c) { probeColor = c;}

    private:
        gcn::Image* probeSpriteImage;
        probe_color probeColor;
        unsigned int probeFrame;
        Uint32 frameTimer;
    };
}

#endif  // BALDERSAMPLEPROBEWIDGET_H
