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

#ifndef SUBMENU_H
#define SUBMENU_H

#include <guichan.hpp>
#include <guichan/sdl.hpp>
#include "menu/widgets/animation.h"
#include "menu/widgets/numberfield.h"
#include "menu/widgets/skinnedbutton.h"
#include "menu/widgets/skinnedcheckbox.h"
#include "menu/widgets/skinnedslider.h"

namespace Balder
{

  class SubMenu:public gcn::ActionListener
  {
  protected:
    SubMenu* parent;
    gcn::Container* page; // holds everything in this submenu

  public:
    SubMenu(gcn::Container* top, SubMenu* parent = 0);
    virtual ~SubMenu()
    {
      delete page;
    }
    /// show this menu
    void show()
    {
      page->setVisible(true);
    }
    /// hide this menu
    void hide()
    {
      page->setVisible(false);
    }

    virtual void action(const std::string& action){}

    Animation* animationPageBackground;
  };

}
#endif // MENU_H
