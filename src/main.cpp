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
 *   Free Software Foundation, Inc.,  .                                     *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_thread.h>
#include <SDL/SDL_mixer.h>
#include <string>
#include "gamemanager.h"
#include "renderer.h"
#include "soundmanager.h"
#include "configmanager.h"
#include "menu/menu.h"
#include "imageloader.h"
#include "filepathmanager.h"
#include "log.h"

using namespace std;
using namespace Balder;

bool setConfig(const char* data_directory = NULL);

int main(int argc, char *argv[])
{
    FilePathManager::Init(argc, argv);
	// first, load the config file
	try{
		Log::output("Loading config manager");
        ConfigManager::LoadConfigFile( FilePathManager::GetConfigPath().c_str() );
	}
	catch (const char *message)
	{
		Log::output("error creating ConfigManager: ");
		Log::output(message);
		fflush(stdout);
		exit(1);
	}

    Log::output("Initializing SDL." );
    /* Initializes Audio and Video */
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK)< 0)
    {
        Log::output("Could not initialize SDL:");
        Log::output(SDL_GetError() );
        SDL_Quit();
    }
    else
    {
        Log::output("Audio & video initialized correctly" );
    }
    atexit(SDL_Quit);
    //Set window cation and icon
    SDL_WM_SetIcon(ImageLoader::LoadImage("balder2d.xpm"), NULL);
    SDL_WM_SetCaption("Balder 2D","balder2d");
    // We want unicode
	SDL_EnableUNICODE(1);

    // create the renderer
    Renderer *render;
    try
    {
        Log::output("creating renderer" );
        render = new Renderer();
    }
    catch (const char *message)
    {
        Log::output(message );
        fflush(stdout);
        exit(1);
    }
    catch(...)
    {
        Log::output("failed to create renderer" );
        exit(1);
    }

	// create the sound manager
	SoundManager* sound;
	try
	{
		Log::output("creating sound manager" );
		sound = new SoundManager();
	}
	catch (const char *message)
	{
		Log::output(message );
		fflush(stdout);
		exit(1);
	}

	catch (const char *message)
	{
		Log::output(message );
		fflush(stdout);
		exit(1);
	}

	// create the menu
	Menu* menu;
	try
	{
		Log::output("creating main menu" );
		menu = new Menu(render, sound);
	}
	catch (const char* message)
	{
		Log::output(message );
		fflush(stdout);
		exit(1);
    }

   	// open the menu, let the user configure the game, then create a
   	// new GameManager for each game, run the game, delete the GameManager
   	// and open the menu again.  Repeat until the user wants to quit
   	while(menu->Run())
   	{
		// hide the mouse pointer
		SDL_ShowCursor(SDL_DISABLE);
   		// create the GameManager
	    GameManager* game_manager;
    	try
    	{
        	game_manager = new GameManager(render, sound);
    	}
    	catch(const char* message)
    	{
			Log::output(message );
			fflush(stdout);
			exit(1);
    	}
    	catch(...)
    	{
        	Log::output("Failed to create GameManager!");
        	exit(1);
    	}
    	Log::output("GameManager Created");

    	try
    	{
		game_manager->Start();
    	}
    	catch (const char *message)
    	{
        	Log::output(message );
        	fflush(stdout);
    	}
        // that game is done, destroy the evidence!
        Log::output("Cleaning up GameManager...");
	    delete game_manager;
	    Log::output("GameManager Destroyed!");
	    // show the mouse pointer
		SDL_ShowCursor(SDL_ENABLE);
		render->SetScreen(800,600);
		Log::output("returning to menu");
   	}
   	// oops, looks like the user doesn't want to play anymore :(
   	// clean up the rest of the crap we created
    Log::output("Cleaning up Menu system...");
   	delete menu;
    delete render;
    delete sound;
    SDL_Quit();
    FilePathManager::Cleanup();
    return 0;
}
