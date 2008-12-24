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

#include "log.h"
#include "mapparser.h"
#include "gamemanager.h"
#include "tinyxml/tinyxml.h"
#include "filepathmanager.h"

using namespace Balder;

MapParser::MapParser(const char* mapname)
{
	std::string filename("maps/");
	filename = filename + mapname + "/" + "maptags.xml";
	filename = FilePathManager::GetFullPath(filename.c_str());
	Log::output("Loading Mapfile " + filename);
	map_file = new TiXmlDocument( filename );
	map_file->LoadFile();
}
MapParser::~MapParser()
{
    delete map_file;
}

const char* MapParser::GetMapMusic()
{
	TiXmlHandle docHandle(map_file);
	TiXmlElement* musicElement = docHandle.FirstChild( "Map" )
        .FirstChild( "Music" ).Element();
    if (musicElement) {
	    const char* mapMusic = musicElement->Attribute("name");
        return mapMusic;
    }
    return "default";
}
int MapParser::GetMapWidth()
{
	TiXmlHandle docHandle(map_file);
	TiXmlElement* size = docHandle.FirstChild( "Map" )
			.FirstChild( "Size" ).Element();
	int n;
	int result = size->QueryIntAttribute("width",&n);
    if (TIXML_SUCCESS == result){ return n;}
    return 0;
}
int MapParser::GetMapHeight()
{
	TiXmlHandle docHandle(map_file);
	TiXmlElement* size = docHandle.FirstChild( "Map" )
			.FirstChild( "Size" ).Element();
	int n;
	int result = size->QueryIntAttribute("height",&n);
    if (TIXML_SUCCESS == result){ return n;}
    return 0;
}
SpawnLocation* MapParser::FirstSpawnLocation()
{
	TiXmlHandle docHandle(map_file);
	spawn = docHandle.FirstChild( "Map" ).FirstChild( "spawn" ).Element();
	if (!spawn) { throw "no spawn locations defined in map!";}
	SpawnLocation *s = new SpawnLocation();
	TiXmlElement* loc = spawn->FirstChildElement("location");
	int result = loc->QueryIntAttribute("x",&(s->x));
    if (TIXML_SUCCESS != result){
    	delete s;
	    throw "spawn location has no x position!";
    }
    result = loc->QueryIntAttribute("y",&(s->y));
    if (TIXML_SUCCESS != result){
        delete s;
	    throw "spawn location has no y position!";
    }
    TiXmlElement* vel = spawn->FirstChildElement("velocity");
    result = vel->QueryDoubleAttribute("x",&(s->x_vel));
    if (TIXML_SUCCESS != result){
        delete s;
	    throw "spawn location has no x velocity!";
    }
    result = vel->QueryDoubleAttribute("y",&(s->y_vel));
    if (TIXML_SUCCESS != result){
        delete s;
	    throw "spawn location has no y velocity!";
    }
    TiXmlElement* stk = spawn->FirstChildElement("stuck");
    std::string str("false");
    const char *stuck_status = stk->Attribute("status");
    s->stuck = str.compare(stuck_status); // returns 0 if they match
    return s;
}
SpawnLocation* MapParser::NextSpawnLocation()
{
	spawn = spawn->NextSiblingElement("spawn");
	if (!spawn) { return 0;}
	SpawnLocation *s = new SpawnLocation();
	TiXmlElement* loc = spawn->FirstChildElement("location");
	int result = loc->QueryIntAttribute("x",&(s->x));
    if (TIXML_SUCCESS != result){
    	delete s;
	    throw "spawn location has no x position!";
    }
    result = loc->QueryIntAttribute("y",&(s->y));
    if (TIXML_SUCCESS != result){
        delete s;
	    throw "spawn location has no y position!";
    }
    TiXmlElement* vel = spawn->FirstChildElement("velocity");
    result = vel->QueryDoubleAttribute("x",&(s->x_vel));
    if (TIXML_SUCCESS != result){
        delete s;
	    throw "spawn location has no x velocity!";
    }
    result = vel->QueryDoubleAttribute("y",&(s->y_vel));
    if (TIXML_SUCCESS != result){
        delete s;
	    throw "spawn location has no y velocity!";
    }
    TiXmlElement* stk = spawn->FirstChildElement("stuck");
    std::string str("false");
    const char *stuck_status = stk->Attribute("status");
    s->stuck = str.compare(stuck_status);
    return s;
}
