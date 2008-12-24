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

#include <SDL/SDL_mixer.h>
#include "menu/menu.h"
#include "menu/mainmenu.h"
#include "soundmanager.h"
#include "imageloader.h"
#include "renderer.h"
#include <iostream>

using namespace Balder;

SDLKey Menu::lastKeyPressed = SDLKey(0);

Menu::Menu(Renderer *r, SoundManager* s): startgame(false), render(r), sound(s)
{
  //screen = SDL_GetVideoSurface();

  imageLoader = new gcn::SDLImageLoader();
  // The ImageLoader in use is static and must be set to be
  // able to load images
  gcn::Image::setImageLoader(imageLoader);
  graphics = new gcn::SDLGraphics();
  // Set the target for the graphics object to be the screen.
  // In other words, we will draw to the screen.
  // Note, any surface will do, it doesn't have to be the screen.
  //graphics->setTarget(screen);
  input = new gcn::SDLInput();

  /*
   * Last but not least it's time to initialize and create the gui
   * with Guichan stuff.
   */
  topContainer = new gcn::Container();
  // Set the dimension of the top container to match the screen.
  topContainer->setDimension(gcn::Rectangle(0, 0, 800, 600));
  gui = new gcn::Gui();
  // Set gui to use the SDLGraphics object.
  gui->setGraphics(graphics);
  // Set gui to use the SDLInput object
  gui->setInput(input);
  // Set the top container
  gui->setTop(topContainer);
  // Load the image font.
  try
  {
    font = ImageLoader::LoadGcnImageFont("menu/balderfont.png");
  }
  catch(gcn::Exception e)
  {
    std::cout << e.getMessage() << std::endl;
  }
  // The global font is static and must be set.
  gcn::Widget::setGlobalFont(font);
  // create top level wigets we want on all menus
  pulseAnimation = new Animation(/* Film Strip File Name */"menu/pulse1.png",
    /* Has Base Frame */true, /* Is Looped */true, /* Play when finished loading */true,
    /* Strip Location */ 0, /* Frame Width */100, /* Frame Height */100, /* Base Frame Index */0,
    /* Starting Frame Index */1, /* Animation Frame Quantity Including Starting Frame */3,
    /* Frame Interval in MS (1Sec == 1000MS) */115);
  twinkleAnimation = new Animation(/* Film Strip File Name */"menu/twinkle1.png",
    /* Has Base Frame */true, /* Is Looped */true, /* Play when finished loading */true,
    /* Strip Location */ 0, /* Frame Width */100, /* Frame Height */100, /* Base Frame Index */0,
    /* Starting Frame Index */1, /* Animation Frame Quantity Including Starting Frame */3,
    /* Frame Interval in MS (1Sec == 1000MS) */1);
  backgroundImage = ImageLoader::LoadGcnImage("menu/menubackground.png");
  backgroundIcon = new gcn::Icon(backgroundImage);
  headerImage = ImageLoader::LoadGcnImage("menu/balderheader.png");
  headerIcon = new gcn::Icon(headerImage);
  topContainer->add(backgroundIcon, 0, 0);
  topContainer->add(pulseAnimation,575,50);
  topContainer->add(twinkleAnimation,600,420);
  topContainer->add(headerIcon, 250, 40);

  // now initialize menus
  mainMenu = new MainMenu(topContainer, this);
  mainMenu->show();
}

Menu::~Menu()
{
  delete font;
  delete gui;
  delete input;
  delete graphics;
  delete imageLoader;
  delete headerIcon;
  delete mainMenu;
  delete topContainer;
}

bool Menu::Run()
{
  screen = SDL_GetVideoSurface();
  graphics->setTarget(screen);
  bool fullscreen;
  running = true;
  while(running)
  {
    // Set fullscreen status for menu.
    if (fullscreen != ConfigManager::GetIntValue("Video", "fullscreen")) {
        fullscreen = ConfigManager::GetIntValue("Video", "fullscreen");
        render->SetScreen(800, 600, fullscreen);
    }

    // Check to see if music is playing.
    if (!sound->IsMusicPlaying())
    {
      sound->PlayMusic("menu");
    }
    // Poll input
    CheckInput();
    // Let the gui perform it's logic (like handle input)
    gui->logic();
    // Draw the gui
    gui->draw();
    // update the screen
    SDL_UpdateRect(screen,0,0,0,0);
  }
  if (startgame)
  {
    return true;
  }
  return false;
}

void Menu::action(const gcn::ActionEvent& action)
{
  // Main menu events.********************************************************
  if (action.getId() == "startgame")
  {
    running = false;
    startgame = true;
  }
  else if (action.getId() == "quitgame")
  {
    running = false;
    startgame = false;
  }
}

void Menu::CheckInput()
{
  /*
   * Poll SDL events
   */
  SDL_Event event;
  while(SDL_PollEvent(&event))
  {
    if (event.type == SDL_KEYDOWN)
    {
        Menu::lastKeyPressed = event.key.keysym.sym;
    }
   else if(event.type == SDL_QUIT)
    {
      running = false;
      startgame = false;
    }
    /*
     * Now that we are done polling and using SDL events we pass
     * the leftovers to the SDLInput object to later be handled by
     * the Gui.
     */
    input->pushInput(event);
  }
}

