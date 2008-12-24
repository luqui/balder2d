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

#include "menu/submenu.h"

using namespace Balder;

SubMenu::SubMenu(gcn::Container* top, SubMenu* parent):parent(parent)
{
  page = new gcn::Container();
  page->setVisible(false);
  page->setOpaque(false);
  page->setDimension(gcn::Rectangle(0, 0, 700, 380));

  animationPageBackground = new Animation(/* Film Strip File Name */"menu/menuskin.png",
    /* Has Base Frame */false, /* Is Looped */true, /* Play when finished loading */true,
    /* Strip Location */0, /* Frame Width */700, /* Frame Height */380, /* Base Frame Index */0,
    /* Starting Frame Index */0, /* Animation Frame Quantity Including Starting Frame */1,
    /* Frame Interval in MS (1Sec == 1000MS) */1000);
  page->add(animationPageBackground, 0, 0);

  top->add(page, 50, 200);
}

