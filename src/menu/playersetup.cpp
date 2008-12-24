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

#include "log.h"
#include "menu/playersetup.h"
#include "menu/menu.h"
#include "configmanager.h"
#include "menu/widgets/sampleprobe.h"
#include "menu/widgets/ailistmodel.h"
#include "menu/widgets/skinneddropdown.h"
#include "input.h"
#include "imageloader.h"
#include <sstream>

using namespace Balder;

PlayerSetupMenu::PlayerSetupMenu(gcn::Container* top, SubMenu* parent):SubMenu(top,parent),currentPlayer(1), bindNextKey(false)
{
    backButton = new SkinnedButton("menu/buttonskins.png", 80, "Back");
    backButton->setWidth(150);
    backButton->setActionEventId("back");
    backButton->addActionListener(this);

    std::stringstream ss;
    ss << "Player " << currentPlayer;
    currentPlayerLabel = new gcn::Label(ss.str());

    nextPlayerButton = new SkinnedButton("menu/buttonskins.png", 120, " > ");
    nextPlayerButton->setActionEventId("nextPlayer");
    nextPlayerButton->addActionListener(this);
    nextPlayerButton->setWidth(40);

    prevPlayerButton = new SkinnedButton("menu/buttonskins.png", 120, " < ");
    prevPlayerButton->setActionEventId("prevPlayer");
    prevPlayerButton->addActionListener(this);
    prevPlayerButton->setWidth(40);

    setRotateRightButton = new SkinnedButton("menu/buttonskins.png", 40, "Rotate Right:");
    setRotateRightButton->setActionEventId("bindRight");
    setRotateRightButton->addActionListener(this);
    setRotateRightButton->addKeyListener(this);// listen for key presses for keybinding stuff
    rotateRightLabel = new gcn::Label(ConfigManager::GetPlayerControl(currentPlayer, "right"));
    setRotateRightButton->setAlignment(gcn::Graphics::LEFT);

    setRotateLeftButton = new SkinnedButton("menu/buttonskins.png", 40, "Rotate Left:");
    setRotateLeftButton->setActionEventId("bindLeft");
    setRotateLeftButton->addActionListener(this);
    setRotateLeftButton->addKeyListener(this);// listen for key presses for keybinding stuff
    rotateLeftLabel = new gcn::Label(ConfigManager::GetPlayerControl(currentPlayer, "left"));
    setRotateLeftButton->setAlignment(gcn::Graphics::LEFT);

    setStickPushButton = new SkinnedButton("menu/buttonskins.png", 40, "Stick/Push Off:");
    setStickPushButton->setActionEventId("bindStick");
    setStickPushButton->addActionListener(this);
    setStickPushButton->addKeyListener(this);// listen for key presses for keybinding stuff
    stickPushLabel = new gcn::Label(ConfigManager::GetPlayerControl(currentPlayer, "stick"));
    setStickPushButton->setAlignment(gcn::Graphics::LEFT);

    setFireButton = new SkinnedButton("menu/buttonskins.png", 40, "Fire:");
    setFireButton->setActionEventId("bindFire");
    setFireButton->addActionListener(this);
    setFireButton->addKeyListener(this);// listen for key presses for keybinding stuff
    fireLabel = new gcn::Label(ConfigManager::GetPlayerControl(currentPlayer, "fire"));
    setFireButton->setAlignment(gcn::Graphics::LEFT);

    setStickPushButton->setWidth(250);
    setRotateRightButton->setWidth(setStickPushButton->getWidth());
    setRotateLeftButton->setWidth(setStickPushButton->getWidth());
    setFireButton->setWidth(setStickPushButton->getWidth());

    // Loads colors from config file to init color sliders.
    probe_color playerColor = ConfigManager::GetPlayerColor(currentPlayer, 255);
    sampleProbe = new SampleProbe();
    sampleProbe->setProbeColor(playerColor);
    playerColor = playerColor >> 8;
    Uint8 cBlue = Uint8(playerColor);
    playerColor = playerColor >> 8;
    Uint8 cGreen = Uint8(playerColor);
    playerColor = playerColor >> 8;
    Uint8 cRed = Uint8(playerColor);

    redImage= ImageLoader::LoadGcnImage("menu/redspectrum.png");
    redIcon = new gcn::Icon(redImage);
    redSlider = new SkinnedSlider("menu/menuskin.png", 32, 0, 255, 10);
    redSlider->setWidth(redIcon->getWidth());
    redSlider->setHeight(15);
    redSlider->setValue(double(cRed));
    redSlider->setActionEventId("colorChange");
    redSlider->addActionListener(this);

    greenImage= ImageLoader::LoadGcnImage("menu/greenspectrum.png");
    greenIcon = new gcn::Icon(greenImage);
    greenSlider = new SkinnedSlider("menu/menuskin.png", 32, 0, 255, 10);
    greenSlider->setWidth(redIcon->getWidth());
    greenSlider->setHeight(15);
    greenSlider->setValue(double(cGreen));
    greenSlider->setActionEventId("colorChange");
    greenSlider->addActionListener(this);

    blueImage= ImageLoader::LoadGcnImage("menu/bluespectrum.png");
    blueIcon = new gcn::Icon(blueImage);
    blueSlider = new SkinnedSlider("menu/menuskin.png", 32, 0, 255, 10);
    blueSlider->setWidth(redIcon->getWidth());
    blueSlider->setHeight(15);
    blueSlider->setValue(double(cBlue));
    blueSlider->setActionEventId("colorChange");
    blueSlider->addActionListener(this);

    // AI Script list box.
    aiLabel = new gcn::Label("AI Script: ");
    ailistModel = new AIListModel();
    aiScrollArea = new SkinnedScrollArea();
    aiListBox = new SkinnedListBox();
    aiListBox->setWidth(redIcon->getWidth() - 12);
    aiListBox->setActionEventId("changeAIScript");
    aiListBox->addActionListener(this);
    aiDropDown = new SkinnedDropDown(ailistModel, aiScrollArea, aiListBox);
    aiDropDown->setWidth(redIcon->getWidth());
    aiDropDown->setSelected(ailistModel->FindScriptIndex(ConfigManager::GetControlType(currentPlayer)));

    page->add(backButton, page->getWidth() - (backButton->getWidth()+50), page->getHeight() - (50 + backButton->getHeight()));
    page->add(prevPlayerButton, 50, 50);
    page->add(currentPlayerLabel, 50+prevPlayerButton->getWidth(), 50);
    page->add(nextPlayerButton, 50+prevPlayerButton->getWidth() + currentPlayerLabel->getWidth(), 50);
    page->add(setRotateRightButton, 50, prevPlayerButton->getY()+70);
    page->add(rotateRightLabel, 50+setRotateRightButton->getWidth(), prevPlayerButton->getY()+70);
    page->add(setRotateLeftButton, 50, prevPlayerButton->getY()+120);
    page->add(rotateLeftLabel, 50+setRotateLeftButton->getWidth(), prevPlayerButton->getY()+120);
    page->add(setStickPushButton, 50, prevPlayerButton->getY()+170);
    page->add(stickPushLabel, 50+setStickPushButton->getWidth(), prevPlayerButton->getY()+170);
    page->add(setFireButton, 50, prevPlayerButton->getY()+220);
    page->add(fireLabel, 50+setFireButton->getWidth(), prevPlayerButton->getY()+220);

    // Color selection controls for the selected player.
    colorTop = 110; // This sets the top most position for the color selection interface.
    colorLeft = page->getWidth() - (redIcon->getWidth()+50); // This sets the left most position for the color selection interface.
    page->add(sampleProbe, colorLeft +(redIcon->getWidth() - sampleProbe->getWidth())/2, colorTop);
    page->add(redIcon, colorLeft, colorTop + sampleProbe->getHeight() + 10);
    page->add(redSlider, colorLeft, colorTop + sampleProbe->getHeight() + 10 + redIcon->getHeight());
    page->add(greenIcon, colorLeft, colorTop + sampleProbe->getHeight() + 10 + redIcon->getHeight() + redSlider->getHeight() + 10);
    page->add(greenSlider, colorLeft, colorTop + sampleProbe->getHeight() + 10 + redIcon->getHeight() + redSlider->getHeight() + 10 + greenIcon->getHeight());
    page->add(blueIcon, colorLeft, colorTop + sampleProbe->getHeight() + 10 + redIcon->getHeight() + redSlider->getHeight() + 10 + greenIcon->getHeight() + greenSlider->getHeight() + 10);
    page->add(blueSlider, colorLeft, colorTop + sampleProbe->getHeight() + 10 + redIcon->getHeight() + redSlider->getHeight() + 10 + greenIcon->getHeight() + greenSlider->getHeight() + 10 + blueIcon->getHeight());

    // AI Script Listbox
    page->add(aiDropDown, colorLeft, nextPlayerButton->getY());
    page->add(aiLabel, aiDropDown->getX() - aiLabel->getWidth(), aiDropDown->getY());
    // lastly call refreshKeys to make sure everything displays correctly
    RefreshKeys();
}

PlayerSetupMenu::~PlayerSetupMenu()
{
}

void PlayerSetupMenu::action(const gcn::ActionEvent& action)
{
    bindNextKey = false; // so key binding gets cancelled when non-bind actions are called.
    if (action.getId() == "nextPlayer")
    {
        if (currentPlayer < ConfigManager::GetNumPlayers())
        {
            ++currentPlayer;
            RefreshKeys();
        }
    }
    else if (action.getId() == "prevPlayer")
    {
        if (currentPlayer > 1 )
        {
            --currentPlayer;
            RefreshKeys();
        }
    }
    else if (action.getId() == "bindRight")
    {
        rotateRightLabel->setCaption(" ");
        actionToBind = "right";
        bindNextKey = true;
    }
    else if (action.getId() == "bindLeft")
    {
        rotateLeftLabel->setCaption(" ");
        actionToBind = "left";
        bindNextKey = true;
    }
    else if (action.getId() == "bindStick")
    {
        stickPushLabel->setCaption(" ");
        actionToBind = "stick";
        bindNextKey = true;
    }
    else if (action.getId() == "bindFire")
    {
        fireLabel->setCaption(" ");
        actionToBind = "fire";
        bindNextKey = true;
    }
    else if (action.getId() == "changeAIScript")
    {
        ConfigManager::SetControlType(currentPlayer, ailistModel->getElementAt(aiListBox->getSelected()).c_str());
        RefreshKeys();
    }
    else if (action.getId() == "colorChange")
    {
        Uint32 rgbColor = Uint8(redSlider->getValue());
        rgbColor = rgbColor << 8;
        rgbColor += Uint8(greenSlider->getValue());
        rgbColor = rgbColor << 8;
        rgbColor += Uint8(blueSlider->getValue());
        sampleProbe->setProbeColor((rgbColor << 8) + 0xFF);
        ConfigManager::SetPlayerColor(currentPlayer, rgbColor);
    }
    else if (action.getId() == "back")
    {
        hide();
        parent->show();
    }
}

void PlayerSetupMenu::RefreshKeys()
{
    int numPlayers = ConfigManager::GetNumPlayers();
    if (currentPlayer > numPlayers) {currentPlayer = numPlayers;}
    std::stringstream ss;
    ss << "Player " << currentPlayer;
    currentPlayerLabel->setCaption(ss.str());
    // Refreshes the AI script selector for the players menu
    aiDropDown->setSelected(ailistModel->FindScriptIndex(ConfigManager::GetControlType(currentPlayer)));
    if (aiDropDown->getSelected()) { // in otherwords != 0, which is human
        // hide all the key binding stuff
        setRotateRightButton->setVisible(false);
        rotateRightLabel->setVisible(false);
        setRotateLeftButton->setVisible(false);
        rotateLeftLabel->setVisible(false);
        setStickPushButton->setVisible(false);
        stickPushLabel->setVisible(false);
        setFireButton->setVisible(false);
        fireLabel->setVisible(false);
    }
    else { // it's a human player, so we want to show keybinging stuff
        setRotateRightButton->setVisible(true);
        rotateRightLabel->setVisible(true);
        setRotateLeftButton->setVisible(true);
        rotateLeftLabel->setVisible(true);
        setStickPushButton->setVisible(true);
        stickPushLabel->setVisible(true);
        setFireButton->setVisible(true);
        fireLabel->setVisible(true);
        rotateRightLabel->setCaption(ConfigManager::GetPlayerControl(currentPlayer, "right"));
        rotateRightLabel->adjustSize();
        rotateLeftLabel->setCaption(ConfigManager::GetPlayerControl(currentPlayer, "left"));
        rotateLeftLabel->adjustSize();
        stickPushLabel->setCaption(ConfigManager::GetPlayerControl(currentPlayer, "stick"));
        stickPushLabel->adjustSize();
        fireLabel->setCaption(ConfigManager::GetPlayerControl(currentPlayer, "fire"));
        fireLabel->adjustSize();
    }

    // Refreshes the color sliders.
    probe_color playerColor = ConfigManager::GetPlayerColor(currentPlayer, 255);
    sampleProbe->setProbeColor(playerColor);
    playerColor = playerColor >> 8;
    Uint8 cBlue = Uint8(playerColor);
    playerColor = playerColor >> 8;
    Uint8 cGreen = Uint8(playerColor);
    playerColor = playerColor >> 8;
    Uint8 cRed = Uint8(playerColor);
    redSlider->setValue(double(cRed));
    greenSlider->setValue(double(cGreen));
    blueSlider->setValue(double(cBlue));
}

void PlayerSetupMenu::keyPressed(gcn::KeyEvent &keyEvent)
{
    int value = keyEvent.getKey().getValue();
    if (value == gcn::Key::ESCAPE)
    {
        bindNextKey = false;
        RefreshKeys();
    }
    else if (bindNextKey && Menu::lastKeyPressed)
    {
        ConfigManager::SetPlayerControl(currentPlayer, actionToBind, Input::GetKeyString(Menu::lastKeyPressed));
        bindNextKey = false;
        RefreshKeys();
    }
}
