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

#ifndef PLAYERSETUPMENU_H
#define PLAYERSETUPMENU_H

#include "submenu.h"

namespace Balder
{
    class AIListModel;
    class SampleProbe;

    class PlayerSetupMenu:public SubMenu, public gcn::KeyListener
    {
    public:
        PlayerSetupMenu(gcn::Container* top, SubMenu* parent);
        virtual ~PlayerSetupMenu();
        void action(const gcn::ActionEvent& action);
        void keyPressed(gcn::KeyEvent &keyEvent);
    private:
        void RefreshKeys();
        SkinnedButton* backButton;
        SkinnedButton* nextPlayerButton;
        SkinnedButton* prevPlayerButton;
        gcn::Label* currentPlayerLabel;
        int currentPlayer;
        bool bindNextKey;
        const char* actionToBind;
        SkinnedButton* setRotateRightButton;
        gcn::Label* rotateRightLabel;
        SkinnedButton* setRotateLeftButton;
        gcn::Label* rotateLeftLabel;
        SkinnedButton* setStickPushButton;
        gcn::Label* stickPushLabel;
        SkinnedButton* setFireButton;
        gcn::Label* fireLabel;
        gcn::Label* aiLabel;
        AIListModel* ailistModel;
        gcn::ScrollArea* aiScrollArea;
        gcn::ListBox* aiListBox;
        gcn::DropDown* aiDropDown;
        gcn::Image* redImage;
        gcn::Icon* redIcon;
        SkinnedSlider* redSlider;
        gcn::Image* greenImage;
        gcn::Icon* greenIcon;
        SkinnedSlider* greenSlider;
        gcn::Image* blueImage;
        gcn::Icon* blueIcon;
        SkinnedSlider* blueSlider;
        SampleProbe* sampleProbe;
        int colorTop;
        int colorLeft;

    };

}
#endif // PLAYERSETUPMENU_H
