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

#ifndef BALDERSAMPLEMAP_H
#define BALDERSAMPLEMAP_H

#include <guichan/widget.hpp>
#include "../../balder2dtypes.h"

namespace Balder
{
    /*
     * An Icon for displaying a sample of the map.
     */
    class SampleMap: public gcn::Widget
    {
    public:
        /*
         * Constructor.
         *
         * @param image an Image to display.
         */
        SampleMap(std::string iniMapName, unsigned int X, unsigned int Y, unsigned int W, unsigned int H);
        ~SampleMap();

        void setSampleMap(std::string name);

        // Inherited from Widget
        virtual void draw(gcn::Graphics* graphics);

    private:
        SDL_Surface* map;
    };
}

#endif  // BALDERSAMPLEMAP_H
