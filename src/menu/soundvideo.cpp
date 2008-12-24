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

#include "menu/soundvideo.h"
#include "configmanager.h"
#include "soundmanager.h"

using namespace Balder;

SoundVideoMenu::SoundVideoMenu(gcn::Container* top, SubMenu* parent):SubMenu(top,parent)
{
    backButton = new SkinnedButton("menu/buttonskins.png", 80, "Back");
    backButton->setWidth(150);
    backButton->setActionEventId("back");
    backButton->addActionListener(this);

    fullScreenCheckbox = new SkinnedCheckBox("menu/menuskin.png", 380, "Fullscreen", ConfigManager::GetIntValue("Video", "fullscreen"));
    fullScreenCheckbox->setActionEventId("toggleFullscreen");
    fullScreenCheckbox->addActionListener(this);

    soundCheckbox = new SkinnedCheckBox("menu/menuskin.png", 380, "Sound", ConfigManager::GetIntValue("Sound", "sound"));
    soundCheckbox->setActionEventId("toggleSound");
    soundCheckbox->addActionListener(this);

    musicCheckbox = new SkinnedCheckBox("menu/menuskin.png", 380, "Music", ConfigManager::GetIntValue("Sound", "music"));
    musicCheckbox->setActionEventId("toggleMusic");
    musicCheckbox->addActionListener(this);

    fxVolumeLabel = new gcn::Label("    Volume");
    fxVolumeSlider = new SkinnedSlider("menu/menuskin.png", 32, 0, 255, 10);
    fxVolumeSlider->setSize(202, (fxVolumeLabel->getHeight()/2)+2);
    fxVolumeSlider->setValue(ConfigManager::GetDoubleValue("Sound", "fxvolume"));
    fxVolumeSlider->setActionEventId("fxVolumeChange");
    fxVolumeSlider->addActionListener(this);

    musicVolumeLabel = new gcn::Label("    Volume");
    musicVolumeSlider = new SkinnedSlider("menu/menuskin.png", 32, 0, 255, 10);
    musicVolumeSlider->setSize(202, (musicVolumeLabel->getHeight()/2)+2);
    musicVolumeSlider->setValue(ConfigManager::GetDoubleValue("Sound", "musicvolume"));
    musicVolumeSlider->setActionEventId("musicVolumeChange");
    musicVolumeSlider->addActionListener(this);

    page->add(backButton, page->getWidth() - (backButton->getWidth()+50), page->getHeight() - (50 + backButton->getHeight()));
    page->add(fullScreenCheckbox, 50, 50);
    page->add(soundCheckbox, 50, 90);
    page->add(musicCheckbox, 50, 130);
    page->add(fxVolumeLabel, 50+soundCheckbox->getWidth(), soundCheckbox->getY());
    page->add(fxVolumeSlider, 50+soundCheckbox->getWidth()+fxVolumeLabel->getWidth()+10, soundCheckbox->getY());
    page->add(musicVolumeLabel, 50+soundCheckbox->getWidth(), musicCheckbox->getY());
    page->add(musicVolumeSlider, 50+soundCheckbox->getWidth()+fxVolumeLabel->getWidth()+10, musicCheckbox->getY());
}

SoundVideoMenu::~SoundVideoMenu()
{
}

void SoundVideoMenu::action(const gcn::ActionEvent& action)
{
    if (action.getId() == "toggleSound")
    {
        ConfigManager::SetIntValue("Sound", "sound", soundCheckbox->isSelected());
    }
    else if (action.getId() == "toggleMusic")
    {
        ConfigManager::SetIntValue("Sound", "music", musicCheckbox->isSelected());
//        sound->PlayMusic("menu");
    }
    else if (action.getId() == "fxVolumeChange")
    {
        Mix_Volume(-1, (int)(MIX_MAX_VOLUME * fxVolumeSlider->getValue()));
        ConfigManager::SetDoubleValue("Sound", "fxvolume", fxVolumeSlider->getValue());
    }
    else if (action.getId() == "musicVolumeChange")
    {
        Mix_VolumeMusic(int(MIX_MAX_VOLUME * musicVolumeSlider->getValue()));
        ConfigManager::SetDoubleValue("Sound", "musicvolume", musicVolumeSlider->getValue());
    }
    else if (action.getId() == "toggleFullscreen")
    {
        ConfigManager::SetIntValue("Video", "fullscreen", fullScreenCheckbox->isSelected());
    }
    else if (action.getId() == "back")
    {
        hide();
        parent->show();
    }
}
