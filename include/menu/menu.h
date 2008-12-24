/***************************************************************************
 *   Copyright (C) 2004 by Bjorn Hansen                                    *
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

#ifndef MENU_H
#define MENU_H

#include <guichan.hpp>
#include <guichan/sdl.hpp>
#include "menu/widgets/animation.h"
#include "menu/widgets/numberfield.h"

namespace Balder
{
class Renderer;
class SoundManager;
class SubMenu;

  class Menu:public gcn::ActionListener
  {
  public:
    Menu(Renderer *, SoundManager *);
    virtual ~Menu();
     void action(const gcn::ActionEvent& action);
    bool Run();
    static SDLKey lastKeyPressed; // ugly hack for keybinding
  private:
    void InitMainMenu();
    void CheckInput();
    bool running; // keeps track of if the menu is open or not
    bool startgame; // if true, start a new game when the menu closes, else quit

    Renderer *render;
    SoundManager *sound;
    SDL_Surface* screen;
    /*
     * Guichan SDL stuff we need
     */
    gcn::SDLInput* input;             // Input driver
    gcn::SDLGraphics* graphics;       // Graphics driver
    gcn::SDLImageLoader* imageLoader; // For loading images
    /*
     * Guichan stuff we need
     */
    gcn::Gui* gui;            // A Gui object - binds it all together
    gcn::ImageFont* font;     // A font
    gcn::Container* topContainer;      // A top container
    // stuff that goes in the top container
    Animation* pulseAnimation; // to display background anitmation
    Animation* twinkleAnimation; // to display background anitmation
    gcn::Icon* headerIcon; // to show a title image
    gcn::Image* headerImage; // the image to use
    gcn::Icon* backgroundIcon;
    gcn::Image* backgroundImage; // the image for the background icon
    // and the main menu buttons (stuff that isn't the same across all submenus)
    SubMenu* mainMenu;
  };
}
#endif // MENU_H
