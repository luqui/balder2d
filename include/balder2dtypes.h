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

#ifndef BALDER2DTYPES_H
#define BALDER2DTYPES_H

#define widgetNotFocused 0
#define widgetFocused 1

#define textDisplaceY 4

#include <SDL/SDL.h>

namespace Balder{

typedef unsigned int entity_id;
typedef unsigned int player_id;
typedef int input_states;
typedef Uint32 probe_color;
typedef unsigned int frame_number;

namespace MenuColors
{
const int HIGHLIGHT = 0x909090;
const int SHADOW = 0x101010;
const int FOCUSED = 0xa0a0a0;
const int UNFOCUSED = 0x080808;
const int bgAlpha = 0x40;
const int fontColor = 0x6a7835;
}
}
#endif
