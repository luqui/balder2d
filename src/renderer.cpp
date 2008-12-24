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
#include "SFont/Font.h"
#include "renderer.h"
#include "entity.h"
#include "imageloader.h"
#include <string>

using namespace Balder;

std::map<std::string,SDL_Surface*> Renderer::sprite_registry;

Renderer::Renderer(): background(0)
{
    SetScreen(800, 600);
	font = new Font(ImageLoader::LoadImage("SFont/scorefont.png"));
}


Renderer::~Renderer()
{
    delete font; // this deletes fontImage as well
    if(background) delete background;
    screen = 0;
}

void Renderer::SetScreen(int width, int height, bool fullscreen)
{
	Uint32 flags;
	if(fullscreen){
		flags = SDL_HWSURFACE | SDL_ANYFORMAT | SDL_FULLSCREEN;
	}
	else {
		flags = SDL_HWSURFACE | SDL_ANYFORMAT;
	}
	screen = SDL_SetVideoMode(width, height, 0, flags);
    if (screen == 0)
    {
        throw "couldn't set vid mode:"; //<< SDL_GetError() << endl;
    }
}

bool Renderer::LoadBackground(const char* mapname)
{
    /* Note that the previous background is not unloaded.  This is ok, as if it
    is the same bacground, the LoadImage call below will just give us the same
    pointer back, if it's different...well worst case is we load one copy of the
    bacground for each map the user plays and have them all in memory
    */
	std::string filename("maps/");
	filename += mapname;
	filename += "/";
	filename += "background.png";
	background = ImageLoader::LoadImage(filename.c_str());
    if (background == 0)
    {
        return false;
    }
    // render the background to the screen
    SDL_BlitSurface(background, 0, screen, 0);
    return true;
}

void Renderer::BlitSurface(int x, int y, int width, int height, SDL_Surface* s)
{
    SDL_Rect source = {0, 0, width, height};
    SDL_Rect target = {x, y, 0, 0};
    SDL_BlitSurface( s, &source, screen, &target);
}

void Renderer::DisplayText(int x, int y, std::string text)
{
	//first, clear the area to be written on (mostly for the game timer)

	EraseArea(x-5,y, font->getTextWidth(text)+10, font->getHeight());
	font->write(screen, text, x, y);
}

void Renderer::EraseArea(int x, int y, int width, int height)
{
    SDL_Rect area = {x, y, width, height};
    SDL_BlitSurface( background, &area, screen, &area);
}

void Renderer::EraseScreen()
{
    EraseArea(0, 0, screen->w, screen->h);
}

void Renderer::DrawEntity (Entity* e)
{
    SDL_SetClipRect(screen,&background->clip_rect);
    e->DrawOnto(screen);
    SDL_SetClipRect(screen,0);
}

void Renderer::EraseEntity ( Entity* e )
{
    SDL_Rect *ebbox = e->GetFrame();
	SDL_Rect source = {e->GetX() - ebbox->w, e->GetY() - ebbox->h,
						2*ebbox->w, 2*ebbox->h};
    SDL_BlitSurface(background, &source, screen, &source);
}

SDL_PixelFormat* Renderer::GetScreenPixelFormat()
{
return screen->format;
}

void Renderer::FinishFrame()
{
    if (!screen) throw "no screen surface!";
    SDL_Flip(screen);
}

/*
 * Register's a sprite for for use
 */
void Renderer::RegisterSpriteType(const char* name, SDL_Surface* sprite)
{
	sprite_registry[name] = sprite;
}

