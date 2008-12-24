/***************************************************************************
 *   Copyright (C) 2005 by Bjorn Hansen                                    *
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

#include "log.h"
#include "menu/widgets/maplistmodel.h"
#include <physfs.h>

using namespace Balder;

MapListModel::MapListModel(): numMaps(0)
{
	FindMaps();
}
MapListModel::~MapListModel()
{}

bool find_file( const std::string map_dir, const std::string & file_name)
{
    char **files = PHYSFS_enumerateFiles(map_dir.c_str());
    char **i;
    for (i = files; *i != NULL; i++){
        std::string f = *i;
        if (file_name == f) {
            PHYSFS_freeList(files);
            return true;
        }
    }
    PHYSFS_freeList(files);
    return false;
}

void MapListModel::FindMaps()
{
    std::string map_dir("maps/");
    char **files = PHYSFS_enumerateFiles(map_dir.c_str());
    char **i;
    std::string mapname;
    Log::output("Importing map list...");
    for (i = files; *i != NULL; i++){
        mapname = *i;
        if (PHYSFS_isDirectory((map_dir+mapname).c_str())){
            // lets try to check that this directory really contains a map
       		if (find_file(map_dir+mapname, "maptags.xml")
       			&& find_file(map_dir+mapname, "geometry.png")
       			&& find_file(map_dir+mapname, "background.png") ) {
      				mapnames.push_back(mapname);
       				++numMaps;
            }
        }
    }
    PHYSFS_freeList(files);
}

std::string Balder::MapListModel::getElementAt (int i)
{
  if (i > numMaps - 1)
  {
    Log::output("tried to get map at index higher than number of maps");
    Log::output("numMaps: "+numMaps);
//    Log::output("i :" +i);
    return "--Map Not Found--";
  }
  return mapnames[i];
}

/*!
    \fn Balder::MapListModel::FindMapIndex(std::string mapname)
 */
int Balder::MapListModel::FindMapIndex(std::string mapname)
{
    for (int i = 0; i < numMaps; ++i) {
    	if (!mapname.compare(mapnames[i])) return i;
    }
    return 0;
}
