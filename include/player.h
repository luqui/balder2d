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

#ifndef PLAYER_H
#define PLAYER_H

#include "balder2dtypes.h"
#include <sstream>

struct SDL_Surface;
class Font;

namespace Balder{

class Probe;
class GameManager;

class Player {
  private:
	GameManager *game_manager;
	static Font* scoreFont;
    static Font* lifeFont;
	static SDL_Surface* scoreFontImage; // the image to use to write the score
    static SDL_Surface* lifeFontImage; // the image to use to write lives left
	static unsigned int numPlayers; // so we can free the font whith the last player
	static std::stringstream stringConverter; // for int to string conversion
    player_id id;
	Probe* probe;
	int score;
  public:
  	Player(player_id, GameManager* gm);
  	~Player();
    void DoControl ( input_states inputs);
    void SetProbe ( Probe* p );
    void AddScore(int toAdd){score += toAdd;}
    int getScore() {return score;}
    const Probe* GetProbe(){return probe;}
    const void DrawStats(SDL_Surface*);

	void SetScore(const int& theValue)
	{
	  score = theValue;
	}


	int GetScore() const
	{
	  return score;
	}

};
}
#endif
