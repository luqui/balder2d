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
#include <SDL/SDL_image.h>
#include <SDL/SDL_gfxPrimitives.h>
#include "player.h"
#include "probe.h"
#include "input.h"
#include "gamemanager.h"
#include "imageloader.h"
#include "SFont/Font.h"
#include <sstream>

using namespace Balder;

Font* Player::scoreFont = 0;
Font* Player::lifeFont = 0;
SDL_Surface* Player::scoreFontImage = 0;
SDL_Surface* Player::lifeFontImage = 0;
unsigned int Player::numPlayers = 0;
std::stringstream Player::stringConverter;

Player::Player(player_id id, GameManager* gm): game_manager(gm), id(id), probe(0), score(0)
{
    if (!scoreFont){
        scoreFontImage = ImageLoader::LoadImage("SFont/scorefont.png");
        scoreFont = new Font(scoreFontImage);
    }
    if (!lifeFont){
        lifeFontImage = ImageLoader::LoadImage("SFont/lifefont.png");
        lifeFont = new Font(lifeFontImage);
    }
    ++numPlayers;
}

Player::~Player()
{
    --numPlayers;
    if(numPlayers == 0){
        delete scoreFont;
        delete lifeFont;
        scoreFont = lifeFont = 0;
//        SDL_FreeSurface(scoreFontImage);
//        SDL_FreeSurface(lifeFontImage);
    }
}
void Player::DoControl ( input_states states )
{
    if (!probe) throw "player has no probe!";
    probe->RotateLeft(states & LEFT);
    probe->RotateRight(states & RIGHT);
    probe->Stick(states & STICK);
if (states & PUSH) {probe->PushOff();}
    if (states & FIRE) {probe->Fire();}
}

void Player::SetProbe ( Probe* p )
{
    probe = p;
}

const void Player::DrawStats(SDL_Surface *s)
{
    Uint32 velocity = SDL_MapRGBA(s->format, 0xff, 0x88, 0x88, 0xff);
    Uint32 scoreBG = SDL_MapRGBA(s->format, 0, 0, 0, 0xff);
    Uint32 probeColor = probe->GetProbeColor();
    SDL_Rect r = {0,0,9,9};
    SDL_FillRect(s, 0, scoreBG);
    rectangleColor(s, 2, 2, s->w-2, s->h-2, probeColor);
    double speed = probe->GetPushOffSpeed();
    if (speed > probe->GetMAXSPEED()) speed = probe->GetMAXSPEED();
    r.x = 10;
    r.y = s->h -10;
    for(double i = 0; i < speed; i += probe->GetMAXSPEED() / 8)
    {
        r.y -= 10;
        SDL_FillRect(s, &r, velocity);
    }
    stringConverter.str("");
    stringConverter << score;
    // align the bottom of the font is 10 pixels above the bottom of the box
    int y = s->h - (scoreFont->getHeight() + 10);
    // and 10 pixelsfrom the right
    int x = s->w - (scoreFont->getTextWidth(stringConverter.str())+10);
    scoreFont->write(s, stringConverter.str(), 50, y );
    // check to see if limited lives should show.
    if (game_manager->getMaxLives() >= 0) {
        // clear the string again to write the lives remaining
        stringConverter.str("");
        stringConverter << game_manager->getMaxLives() - probe->GetDeaths();
        // align the bottom of the font is 10 pixels above the top of the score
        y = s->h - (scoreFont->getHeight() + lifeFont->getHeight() + 10);
        // and 10 pixelsfrom the right
        x = s->w - (lifeFont->getTextWidth(stringConverter.str())+10);
        lifeFont->write(s, stringConverter.str(), 50, y);
    }
}
