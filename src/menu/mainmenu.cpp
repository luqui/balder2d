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

#include "menu/menu.h"
#include "menu/mainmenu.h"
#include "menu/options.h"

using namespace Balder;

MainMenu::MainMenu(gcn::Container* top, Menu* menusystem):SubMenu(top),system(menusystem)
{
  startButton = new SkinnedButton("menu/buttonskins.png", 0, "Start Game");
  startButton->setActionEventId("startgame");
  startButton->addActionListener(this);
  startButton->setWidth(300);
  startButton->setAlignment(gcn::Graphics::LEFT);

  optionsButton = new SkinnedButton("menu/buttonskins.png", 0, "Options");
  optionsButton->setActionEventId("optionsMenu");
  optionsButton->addActionListener(this);
  optionsButton->setWidth(300);
  optionsButton->setAlignment(gcn::Graphics::LEFT);

  quitButton = new SkinnedButton("menu/buttonskins.png", 0, "Quit");
  quitButton->setWidth(300);
  quitButton->setActionEventId("quitgame");
  quitButton->addActionListener(this);
  quitButton->setAlignment(gcn::Graphics::LEFT);

  versionLabel = new gcn::Label("Version 1.0");

  // Add them to the page container
  page->add(startButton, 50, 50);
  page->add(optionsButton, 50, 100);
  page->add(quitButton, 50, 150);
  page->add(versionLabel, page->getWidth() - (50 + versionLabel->getWidth()), page->getHeight() - (50 + versionLabel->getHeight()));

  // create the submenus
  optionsMenu = new OptionsMenu(top, this);
}

MainMenu::~MainMenu()
{

}

void MainMenu::action(const gcn::ActionEvent& action)
{
  // Main menu events.********************************************************
  if (action.getId() == "optionsMenu")
  {
    hide();
    optionsMenu->show();
  }
  else  system->action(action);
}
