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

#ifndef OPTIONSMENU_H
#define OPTIONSMENU_H

#include "submenu.h"

namespace Balder
{

  class OptionsMenu:public SubMenu
  {
  public:
    OptionsMenu(gcn::Container* top, SubMenu* parent);
    virtual ~OptionsMenu();
    void action(const gcn::ActionEvent& action);
  private:
    SubMenu* gameSetupMenu;
    SubMenu* playerSetupMenu;
    SubMenu* soundVideoMenu;
    SkinnedButton* backButton;
    SkinnedButton* gameSetupButton;
    SkinnedButton* soundVideoButton;
    SkinnedButton* playersButton;
    gcn::Label* numPlayersLabel; // to display current number of local players
    int numPlayers;
    SkinnedButton* morePlayersButton;
    SkinnedButton* lessPlayersButton;
  };
}
#endif // OPTIONSMENU_H
