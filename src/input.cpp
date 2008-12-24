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

#include <SDL/SDL.h>
#include <SDL/SDL_keyboard.h>
#include <map>
#include <string>
#include "input.h"
#include "gamemanager.h"
#include "configmanager.h"
#include "player.h"
#include "log.h"

using namespace Balder;

const struct {
	const char *s;
	SDLKey k;
} SDLKeyStringTable[] = {
	{"BACKSPACE", SDLK_BACKSPACE},
	{"TAB",       SDLK_TAB},
	{"ENTER",     SDLK_RETURN},
	{"PAUSE",     SDLK_PAUSE},
	{"ESC",       SDLK_ESCAPE},
	{"SPACE",     SDLK_SPACE},
	{"DEL",       SDLK_DELETE},
	{"KP0",       SDLK_KP0},
	{"KP1",       SDLK_KP1},
	{"KP2",       SDLK_KP2},
	{"KP3",       SDLK_KP3},
	{"KP4",       SDLK_KP4},
	{"KP5",       SDLK_KP5},
	{"KP6",       SDLK_KP6},
	{"KP7",       SDLK_KP7},
	{"KP8",       SDLK_KP8},
	{"KP9",       SDLK_KP9},
	{"KP.",       SDLK_KP_PERIOD},
	{"KP/",       SDLK_KP_DIVIDE},
	{"KP*",       SDLK_KP_MULTIPLY},
	{"KP-",       SDLK_KP_MINUS},
	{"KP+",       SDLK_KP_PLUS},
	{"KP_ENTER",  SDLK_KP_ENTER},
	{"UP",        SDLK_UP},
	{"DOWN",      SDLK_DOWN},
	{"RIGHT",     SDLK_RIGHT},
	{"LEFT",      SDLK_LEFT},
	{"INSERT",    SDLK_INSERT},
	{"HOME",      SDLK_HOME},
	{"END",       SDLK_END},
	{"PAGEUP",    SDLK_PAGEUP},
	{"PAGEDOWN",  SDLK_PAGEDOWN},
	{"F1",        SDLK_F1},
	{"F2",        SDLK_F2},
	{"F3",        SDLK_F3},
	{"F4",        SDLK_F4},
	{"F5",        SDLK_F5},
	{"F6",        SDLK_F6},
	{"F7",        SDLK_F7},
	{"F8",        SDLK_F8},
	{"F9",        SDLK_F9},
	{"F10",       SDLK_F10},
	{"F11",       SDLK_F11},
	{"F12",       SDLK_F12},
	{"F13",       SDLK_F13},
	{"F14",       SDLK_F14},
	{"F15",       SDLK_F15},
	{"RSHIFT",	  SDLK_RSHIFT},
	{"LSHIFT",    SDLK_LSHIFT},
	{"RCTRL", 	  SDLK_RCTRL},
	{"LCTRL",	  SDLK_LCTRL},
	{"RALT", 	  SDLK_RALT},
	{"LALT",	  SDLK_LALT},
	{"", SDLK_UNKNOWN} // terminator
};

typedef std::map<std::string, SDLKey> ParseKeyMap;
typedef std::map<std::string, SDLKey>::iterator ParseKeyIterator;
static ParseKeyMap parse_key_map;

bool Input::keymapInitialized = false;

// contstructor
Input::Input(GameManager* gm, int nplayers):game_manager(gm), num_players(nplayers)
{
    SDL_JoystickEventState(SDL_ENABLE);
    joystick = SDL_JoystickOpen(0); // joystick 0
	controllers = new PlayerController[num_players];
	BindKeys();
}

Input::~Input()
{
    delete[] controllers;
}

// checks the input state and pass it along to the game manager
void Input::CheckInput()
{
    SDL_Event event;
    if(SDL_PollEvent(&event))
    {
    	SDLKey k;
    	input_states old;
		switch(event.type)
        {
        	case SDL_QUIT:
            	game_manager->Quit();
            	exit(2);
                break;
            case SDL_KEYDOWN:
            	k = event.key.keysym.sym;
            	if(SDLK_ESCAPE == k)
            		game_manager->Quit();
                if(SDLK_PAUSE == k)
                    game_manager->TogglePause();
            	// now check if the key is mapped to any player action
            	for (int i = 0; i < num_players; ++i)
            	{
            		old = controllers[i].inputs;
            		if (k == controllers[i].right)
            			controllers[i].inputs = old | RIGHT;
            		if (k == controllers[i].left)
            			controllers[i].inputs = old | LEFT;
            		if (k == controllers[i].stick)
            			controllers[i].inputs = old | STICK;
            		if (k == controllers[i].push)
            			controllers[i].inputs = old | PUSH;
            		if (k == controllers[i].fire)
            			controllers[i].inputs = old | FIRE;
            		// if anything was changed, send an event to the gamemanager
            		if (old != controllers[i].inputs){
            			game_manager->SendInputState(
            				controllers[i].id, controllers[i].inputs);
            		}
            	}
            	break;
            case SDL_KEYUP:
            	k = event.key.keysym.sym;
            	// check if the key is mapped to any player action
            	for (int i = 0; i < num_players; ++i)
            	{
            		old = controllers[i].inputs;
            		if (k == controllers[i].right)
            			controllers[i].inputs = old - RIGHT;
            		if (k == controllers[i].left)
            			controllers[i].inputs = old - LEFT;
            		if (k == controllers[i].stick)
            			controllers[i].inputs = old - STICK;
            		if (k == controllers[i].push)
            			controllers[i].inputs = old - PUSH;
            		if (k == controllers[i].fire)
            			controllers[i].inputs = old - FIRE;
            		// if anything was changed, send an event to the gamemanager
            		if (old != controllers[i].inputs){
            			game_manager->SendInputState(
            				controllers[i].id, controllers[i].inputs);
            		}
            	}
            	break;
            case SDL_JOYBUTTONDOWN: // hard coded to controller one, button0=fire, button1=push/stick
                old = controllers[1].inputs;
                if(event.jbutton.button == 0)
                    controllers[1].inputs = old | FIRE;
                else if(event.jbutton.button == 1)
                    controllers[1].inputs = old | STICK;
                else break;
                // we know at this point that button 0 or 1 was pressed,
                // so send the event
                game_manager->SendInputState(controllers[1].id, controllers[1].inputs);
                break;
            case SDL_JOYBUTTONUP:
                old = controllers[1].inputs;
                if(event.jbutton.button == 0 && (old & FIRE))
                    controllers[1].inputs = old - FIRE;
                else if(event.jbutton.button == 1 && (old & STICK))
                    controllers[1].inputs = old - STICK;
                else break;
                // we know at this point that button 0 or 1 was released, so send the event
                game_manager->SendInputState(controllers[1].id, controllers[1].inputs);
                break;
            case SDL_JOYAXISMOTION:
                if (event.jaxis.axis == 0){ // left/right
                    old = controllers[1].inputs;
                    if(event.jaxis.value < -3200){ // left
                        controllers[1].inputs = old | LEFT;
                        if (old & RIGHT)
                            controllers[1].inputs = old - RIGHT;
                    }
                    else if(event.jaxis.value > 3200){ // right
                        controllers[1].inputs = old | RIGHT;
                        if (old & LEFT)
                            controllers[1].inputs = old - LEFT;
                    }
                    else {
                        if (old & RIGHT)
                            controllers[1].inputs = old - RIGHT;
                        if (old & LEFT)
                            controllers[1].inputs = old - LEFT;
                    }
                }
                // if anything was changed, send an event to the gamemanager
            	if (old != controllers[1].inputs){
            		game_manager->SendInputState(
            			controllers[1].id, controllers[1].inputs);
            	}
                break;
            default:
            	return;
        }
    }
}


void Input::SetPlayerInputController(player_id id, int controller_number)
{
	if (controller_number <= num_players) {
		controllers[controller_number -1].id = id;
	}
}

void Input::MapKeys()
{
	// fill the parse map
	int i;
	// put in all named keys
	for (i = 0; strlen(SDLKeyStringTable[i].s) > 0; i++)
	{
		parse_key_map[SDLKeyStringTable[i].s] = SDLKeyStringTable[i].k;
	}
	// add 1 character keys
	for (i = 33; i <123; i++)
	{
		// skip upper case letters
		if (65 == i) i = 91;
		// there has to be a cleaner way to do this but. . .
		std::string s("a");
 		s[0] = (char)i;
		const char* c = s.c_str();
		parse_key_map[c] = (SDLKey)i;
	}
	keymapInitialized = true;
}

const char* Input::GetKeyString(SDLKey k)
{
	if (!keymapInitialized) { MapKeys();}
	ParseKeyIterator itr = parse_key_map.begin();
	while ((*itr).second != k) {
		++itr;
		if (itr == parse_key_map.end()){ return "none";}
	}
	return (*itr).first.c_str();
}


void Input::BindKeys()
{
	if (!keymapInitialized) { MapKeys();}
	// now get all of the keys from the config manager.
	const char* key;
	SDLKey k;
	for (int i = 0; i < num_players; ++i)
	{
		// It is convenient to set the initial input state here so we will
		controllers[i].inputs = 0;
		// remember, players are numbered starting at 1, but the controllers
		// array is indexed 0 to (num_players - 1), so the controller number
		// we pass to the config manager is i + 1.
		key = ConfigManager::GetPlayerControl(i+1,"right");
		if (key && parse_key_map.count(key)) {k = parse_key_map[key];}
		else {k = SDLK_UNKNOWN;}
		controllers[i].right = k;

		key = ConfigManager::GetPlayerControl(i+1,"left");
		if (key && parse_key_map.count(key)) {k = parse_key_map[key];}
		else {k = SDLK_UNKNOWN;}
		controllers[i].left = k;

		key = ConfigManager::GetPlayerControl(i+1,"stick");
		if (key && parse_key_map.count(key)) {k = parse_key_map[key];}
		else {k = SDLK_UNKNOWN;}
		controllers[i].stick = k;

		key = ConfigManager::GetPlayerControl(i+1,"push");
		if (key && parse_key_map.count(key)) {k = parse_key_map[key];}
		else {k = SDLK_UNKNOWN;}
		controllers[i].push = k;

		key = ConfigManager::GetPlayerControl(i+1,"fire");
		if (key && parse_key_map.count(key)) {k = parse_key_map[key];}
		else {k = SDLK_UNKNOWN;}
		controllers[i].fire = k;
	}
}


