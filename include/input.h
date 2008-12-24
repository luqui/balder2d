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

#ifndef INPUT_H
#define INPUT_H

#include "balder2dtypes.h"

namespace Balder{

const int LEFT = 1;
const int RIGHT = 2;
const int STICK = 4;
const int PUSH = 8;
const int FIRE = 16;

struct GameManager;

struct PlayerController{
	player_id id;
	SDLKey right;
	SDLKey left;
	SDLKey stick;
	SDLKey push;
	SDLKey fire;
	input_states inputs;
};

class Input {
  public:
    // contstructor
	Input(GameManager *gm, int num_players);
	~Input();
	// checks the input state and pass it along to the game manager
	void CheckInput();
	void SetPlayerInputController(player_id id, int controller_number);
	static const char* GetKeyString(SDLKey k);
  private:
  	static bool keymapInitialized;
  	static void MapKeys();
  	void BindKeys();
    GameManager *game_manager;
    const int num_players;
    PlayerController* controllers;
    SDL_Joystick *joystick;
};
}
#endif
