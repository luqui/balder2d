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

#ifndef BALDERSKINNEDCHECKBOX_H
#define BALDERSKINNEDCHECKBOX_H

#define checkboxUnChecked 1
#define checkboxChecked 2

#include <guichan/widgets/checkbox.hpp>
#include "../../balder2dtypes.h"

namespace Balder
{
    /**
     * A check box with a skin applied to it's surface.
     */
    class SkinnedCheckBox: public gcn::CheckBox
    {
    public:
        // Constructor and Destructor.
        SkinnedCheckBox(std::string, unsigned int, std::string, bool = false);
        ~SkinnedCheckBox();

        // Inherited from gcn::Widget
        virtual void draw(gcn::Graphics* graphics);
        virtual void drawBorder(gcn::Graphics* graphics);

        // CheckBox control functions
        void setSizeXY(int newSizeXY) {checkboxSizeXY = newSizeXY;};
        int getSizeXY() {return checkboxSizeXY;};

    private:
        // Skin Variables.
        SDL_Surface* skinFrameStrip;
        std::string skinFileName;
        unsigned int skinStripLocation;

        // Widget Variables.
        unsigned int widgetState;
        unsigned int checkboxSizeXY;
    };
}

#endif  // BALDERSKINNEDCHECKBOX_H
