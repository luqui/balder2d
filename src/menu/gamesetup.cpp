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

#include "menu/gamesetup.h"
#include "configmanager.h"
#include "menu/widgets/samplemap.h"
#include "menu/widgets/maplistmodel.h"
#include "menu/widgets/skinneddropdown.h"

using namespace Balder;

GameSetupMenu::GameSetupMenu(gcn::Container* top, SubMenu* parent):SubMenu(top,parent)
{
    backButton = new SkinnedButton("menu/buttonskins.png", 80, "Back");
    backButton->setWidth(150);
    backButton->setActionEventId("back");
    backButton->addActionListener(this);

    sampleMap = new SampleMap(ConfigManager::GetMap(), 0, 0, 100, 80);

    mapnameLabel = new gcn::Label("Select a Map: ");
    maplistModel = new MapListModel();
    mapScrollArea = new SkinnedScrollArea();
    mapListBox = new SkinnedListBox();
    mapListBox->setWidth(400);
    mapListBox->setActionEventId("selectNewMap");
    mapListBox->addActionListener(this);
    mapDropDown = new SkinnedDropDown(maplistModel, mapScrollArea, mapListBox);
    mapDropDown->setWidth(400);
    mapDropDown->setSelected(maplistModel->FindMapIndex(ConfigManager::GetMap()));

    int score = ConfigManager::GetIntValue("GameSetup", "ScoreLimit");
    int time = ConfigManager::GetIntValue("GameSetup", "TimeLimit");
    int life = ConfigManager::GetIntValue("GameSetup", "LifeLimit");
    scoreLimitSelector = new SkinnedCheckBox("menu/menuskin.png", 380, "Maximum Score", score > 0);
    scoreLimitSelector->setActionEventId("toggleScoreLimit");
    scoreLimitSelector->addActionListener(this);
    timeLimitSelector = new SkinnedCheckBox("menu/menuskin.png", 380, "Time (minutes)", score > 0);
    timeLimitSelector->setActionEventId("toggleTimeLimit");
    timeLimitSelector->addActionListener(this);
    lifeLimitSelector = new SkinnedCheckBox("menu/menuskin.png", 380, "Limited Lives", score > 0);
    lifeLimitSelector->setActionEventId("toggleLifeLimit");
    lifeLimitSelector->addActionListener(this);
    scoreLimitValue = new NumberField(score);
    scoreLimitValue->setEnabled(score > 0);
    scoreLimitValue->setVisible(score > 0);
    timeLimitValue = new NumberField(time);
    timeLimitValue->setEnabled(time > 0);
    timeLimitValue->setVisible(time > 0);
    lifeLimitValue = new NumberField(life);
    lifeLimitValue->setEnabled(life > 0);
    lifeLimitValue->setVisible(life > 0);

    page->add(backButton, page->getWidth() - (backButton->getWidth()+50), page->getHeight() - (50 + backButton->getHeight()));
    page->add(sampleMap, 50, 50);
    page->add(mapnameLabel, 50+(sampleMap->getWidth() + 10), 55);
    page->add(mapDropDown, mapnameLabel->getX(), mapnameLabel->getY()+40);
    page->add(scoreLimitSelector, 50, sampleMap->getY() + 88);
    page->add(scoreLimitValue, 400, sampleMap->getY() + 88);
    page->add(timeLimitSelector, 50, scoreLimitSelector->getY() + 40);
    page->add(timeLimitValue, 400, scoreLimitSelector->getY() + 40);
    page->add(lifeLimitSelector, 50, scoreLimitSelector->getY() + 80);
    page->add(lifeLimitValue, 400, scoreLimitSelector->getY() + 80);

}

GameSetupMenu::~GameSetupMenu()
{}

void GameSetupMenu::action(const gcn::ActionEvent& action)
{
    if (action.getId() == "selectNewMap")
    {
        ConfigManager::SetMap(maplistModel->getElementAt(mapListBox->getSelected()).c_str());
        sampleMap->setSampleMap(ConfigManager::GetMap());
    }
    else if (action.getId() == "toggleScoreLimit")
    {
        int score = scoreLimitValue->getValue()*-1;
        if (0 == score) score = -10;
        scoreLimitSelector->setSelected(score > 0);
        scoreLimitValue->setValue(score);
        scoreLimitValue->setEnabled(score > 0);
        scoreLimitValue->setVisible(score > 0);
    }
    else if (action.getId() == "toggleTimeLimit")
    {
        int time = timeLimitValue->getValue()*-1;
        if (0 == time) time = 10;
        timeLimitSelector->setSelected(time > 0);
        timeLimitValue->setValue(time);
        timeLimitValue->setEnabled(time > 0);
        timeLimitValue->setVisible(time > 0);
    }
    else if (action.getId() == "toggleLifeLimit")
    {
        int life = lifeLimitValue->getValue()*-1;
        if (0 == life) life = -10;
        lifeLimitSelector->setSelected(life > 0);
        lifeLimitValue->setValue(life);
        lifeLimitValue->setEnabled(life > 0);
        lifeLimitValue->setVisible(life > 0);
    }
    else if (action.getId() == "back")
    {
        ConfigManager::SetIntValue("GameSetup", "ScoreLimit", scoreLimitValue->getValue());
        ConfigManager::SetIntValue("GameSetup", "TimeLimit", timeLimitValue->getValue());
        ConfigManager::SetIntValue("GameSetup", "LifeLimit", lifeLimitValue->getValue());
        hide();
        parent->show();
    }
}
