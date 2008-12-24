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

#ifndef RENDERER_H
#define RENDERER_H

#include <map>
#include <string>
#include "balder2dtypes.h"

struct SDL_Surface;
class Font;


namespace Balder{

class Entity;

class Renderer {
    public:
        static std::map<std::string,SDL_Surface*> sprite_registry;
        Renderer();
        ~Renderer();
        /// set the dimensionsof the render window
        void SetScreen(int width, int height, bool fullscreen=false);
        /// load the bacground for the map with the specified name.
        bool LoadBackground(const char* mapname);
        /// convenience function to blit an sdl surface to the screen
        void BlitSurface(int x, int y, int width, int height, SDL_Surface* s);
        /// display text on the screen
        void DisplayText(int x, int y, std::string text);
        /// erase a rectangle from the screen, restoring the background
        void EraseArea(int x, int y, int width, int height);
        void EraseScreen();
        /// draws an entity if the associated sprite is registered
        void DrawEntity ( Entity*  );
		/// Register a sprite for for use
		void RegisterSpriteType(const char* name, SDL_Surface* sprite);
        void EraseEntity ( Entity*  );
        SDL_PixelFormat* GetScreenPixelFormat();
        void FinishFrame();
    private:
        SDL_Surface* screen;
        SDL_Surface* background;
        Font* font;
};

}
#endif
