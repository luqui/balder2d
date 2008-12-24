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
#include "menu/widgets/skinnedbutton.h"
#include "imageloader.h"
using namespace Balder;

SkinnedButton::SkinnedButton(std::string sFN, unsigned int sSL, std::string bC):
        gcn::Button(bC), skinFileName(sFN), skinStripLocation(sSL), widgetState(widgetNotFocused)
{
    setFrameSize(0);

    // Load Skin Frame Strip from File.
    skinFrameStrip = ImageLoader::LoadImage(skinFileName.c_str());
    if (skinFrameStrip == 0) {
        throw "could not load button skin!";
    }
}

SkinnedButton::~SkinnedButton()
{
    if (skinFrameStrip != 0) {
        SDL_FreeSurface(skinFrameStrip);
    }
}

void SkinnedButton::draw(gcn::Graphics* graphics)
{
    // get screen that the menu is drawn on.
    SDL_Surface *screen = dynamic_cast<gcn::SDLGraphics*>(graphics)->getTarget();

    // get location of the widget on the menu screen.
    int x,y;
    gcn::Widget::getAbsolutePosition(x,y);

    if (!isFocused()) {
        widgetState = widgetNotFocused;
    }
    else {
        widgetState = widgetFocused;
    }

    if (isPressed()) {
        widgetState += buttonDown;
    }

    // draw widget onto menu screen at widget location.
    SDL_Rect rectSrc = {widgetState*getWidth(), skinStripLocation, getWidth(), getHeight()};
    SDL_Rect rectDest = {x, y, /*width is ignored*/0, /*height is ignored*/0};
    SDL_BlitSurface(skinFrameStrip, &rectSrc, screen, &rectDest);

    // Need to draw caption label onto button.
    int textX;
    int textY = getHeight() / 2 - getFont()->getHeight() / 2;

    switch (getAlignment())
    {
        case gcn::Graphics::LEFT:
            textX = 7;
            break;
        case gcn::Graphics::CENTER:
            textX = getWidth() / 2;
            break;
        case gcn::Graphics::RIGHT:
            textX = getWidth() - 7;
            break;
        default:
            throw GCN_EXCEPTION("Unknown alignment.");
    }

    graphics->setFont(getFont());

    if (isPressed())
    {
        graphics->drawText(getCaption(), textX + 1, textY + 1 + textDisplaceY, getAlignment());
    }
    else
    {
        graphics->drawText(getCaption(), textX, textY + textDisplaceY, getAlignment());
    }
}

void SkinnedButton::drawBorder(gcn::Graphics* graphics)
{
    // Borders are not drawn for skinned buttons.
}
