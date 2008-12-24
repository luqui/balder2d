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

#include "menu/options.h"
#include "menu/gamesetup.h"
#include "menu/playersetup.h"
#include "menu/soundvideo.h"
#include "configmanager.h"
#include <sstream>

using namespace Balder;

OptionsMenu::OptionsMenu(gcn::Container* top, SubMenu* parent):SubMenu(top,parent)
{
    backButton = new SkinnedButton("menu/buttonskins.png", 80, "Back");
    backButton->setWidth(150);
    backButton->setActionEventId("back");
    backButton->addActionListener(this);

    std::stringstream ss;
    numPlayers = ConfigManager::GetNumPlayers();
    ss << "Number of Players: " << numPlayers;
    numPlayersLabel = new gcn::Label(ss.str());

    morePlayersButton = new SkinnedButton("menu/buttonskins.png", 0, "+");
    morePlayersButton->setActionEventId("morePlayers");
    morePlayersButton->addActionListener(this);
    morePlayersButton->setHeight(numPlayersLabel->getHeight()/2);

    lessPlayersButton = new SkinnedButton("menu/buttonskins.png", 0, "-");
    lessPlayersButton->setActionEventId("lessPlayers");
    lessPlayersButton->addActionListener(this);
    lessPlayersButton->setHeight(numPlayersLabel->getHeight()/2);
    lessPlayersButton->setWidth(morePlayersButton->getWidth());

    soundVideoButton = new SkinnedButton("menu/buttonskins.png", 0, "Sound & Video");
    soundVideoButton->setActionEventId("soundVideoMenu");
    soundVideoButton->addActionListener(this);
    soundVideoButton->setWidth(300);
    soundVideoButton->setAlignment(gcn::Graphics::LEFT);

    gameSetupButton = new SkinnedButton("menu/buttonskins.png", 0, "Game Setup");
    gameSetupButton->setActionEventId("gameSetupMenu");
    gameSetupButton->addActionListener(this);
    gameSetupButton->setWidth(soundVideoButton->getWidth());
    gameSetupButton->setAlignment(gcn::Graphics::LEFT);

    playersButton = new SkinnedButton("menu/buttonskins.png", 0, "Player Setup");
    playersButton->setActionEventId("playerSetupMenu");
    playersButton->addActionListener(this);
    playersButton->setWidth(soundVideoButton->getWidth());
    playersButton->setAlignment(gcn::Graphics::LEFT);

    // add these things to the page container
    page->add(backButton, page->getWidth() - (backButton->getWidth()+50), page->getHeight() - (50 + backButton->getHeight()));
    page->add(numPlayersLabel, 50, 50);
    page->add(morePlayersButton, 50+numPlayersLabel->getWidth(), 50);
    page->add(lessPlayersButton, 50+numPlayersLabel->getWidth(), 50+morePlayersButton->getHeight());
    page->add(gameSetupButton, 50, 100);
    page->add(playersButton, 50, 150);
    page->add(soundVideoButton, 50, 200);
    // create the submenus
    gameSetupMenu = new GameSetupMenu(top, this);
    playerSetupMenu = new PlayerSetupMenu(top, this);
    soundVideoMenu = new SoundVideoMenu(top, this);
}

OptionsMenu::~OptionsMenu()
{
}

void OptionsMenu::action(const gcn::ActionEvent& action)
{
    if (action.getId() == "morePlayers")
    {
        if (numPlayers < 8)
        {
            ++numPlayers;
            std::stringstream ss;
            ss << "Number of Players: " << numPlayers;
            numPlayersLabel->setCaption(ss.str());
            ConfigManager::SetNumPlayers(numPlayers);
        }
    }
    else if (action.getId() == "lessPlayers")
    {
        if (numPlayers > 1)
        {
            --numPlayers;
            std::stringstream ss;
            ss << "Number of Players: " << numPlayers;
            numPlayersLabel->setCaption(ss.str());
            ConfigManager::SetNumPlayers(numPlayers);
        }
    }
    else if (action.getId() == "gameSetupMenu")
    {
        hide();
        gameSetupMenu->show();
    }
    else if (action.getId() == "playerSetupMenu")
    {
        hide();
        playerSetupMenu->show();
    }
    else if (action.getId() == "soundVideoMenu")
    {
        hide();
        soundVideoMenu->show();
    }
    else if (action.getId() == "back")
    {
        hide();
        ConfigManager::SaveConfigFile();
        parent->show();
    }
}
