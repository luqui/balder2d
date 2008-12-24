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

#ifndef MAPPARSER_H
#define MAPPARSER_H

class TiXmlDocument;
class TiXmlElement;

namespace Balder{
	
struct SpawnLocation;
	
class MapParser{
public:

	MapParser(const char* mapname);
	virtual ~MapParser();
	const char* GetMapMusic();
	int GetMapWidth();
	int GetMapHeight();
	SpawnLocation* FirstSpawnLocation();
	SpawnLocation* NextSpawnLocation();
private:
	TiXmlDocument *map_file;
	TiXmlElement *spawn;
};
}
#endif // MAPPARSER_H
