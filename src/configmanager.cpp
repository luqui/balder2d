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

#include "configmanager.h"
#include "renderer.h"
#include "tinyxml/tinyxml.h"
#include <fstream>

using namespace Balder;

TiXmlDocument* ConfigManager::config_file = 0;

void error()
{
    throw "\n\n**Your config file is busted, please delete it replace it with a valid one.**\n ";
}

void ConfigManager::LoadConfigFile( )
{
	if (!config_file){
		config_file = new TiXmlDocument( "config.xml" );
        if (!config_file->LoadFile()){ GenerateConfig("config.xml"); }
        config_file->LoadFile();
	}
}

void ConfigManager::LoadConfigFile(const char* filename)
{
	config_file = new TiXmlDocument( filename );
    if (!config_file->LoadFile()){ GenerateConfig(filename); }
    config_file->LoadFile();
}

void ConfigManager::SaveConfigFile()
{
	config_file->SaveFile();
}

const char* ConfigManager::GetPlayerControl(int controller, const char* action)
{
	TiXmlHandle docHandle( config_file );
    TiXmlElement* key = docHandle.FirstChild( "Balder" ).FirstChild( "Player" ).Element();
    while(key)
    {
    	int c;
    	int result = key->QueryIntAttribute("controller",&c);
    	if (TIXML_SUCCESS == result){
    		if (controller == c){
    			TiXmlHandle h(key);
    			TiXmlElement *actEl = h.FirstChild("Actions").FirstChild(action).Element();
    			if (!actEl) { return "none";}
    			const char* keystring = actEl->Attribute("key");
    			if (keystring) {return keystring;}
    		}
    	}
	    key=key->NextSiblingElement("Player");
    }
    return "none";
}

void ConfigManager::SetPlayerControl(int controller, const char* action, const char* key)
{
	TiXmlHandle docHandle( config_file );
    TiXmlElement* player = docHandle.FirstChild( "Balder" ).FirstChild( "Player" ).Element();
    TiXmlElement* lastPlayer = 0;
    while(player)
    {
    	lastPlayer = player;
    	int c;
    	int result = player->QueryIntAttribute("controller",&c);
    	if (TIXML_SUCCESS == result){
    		if (controller == c){
    			TiXmlHandle h(player);
    			TiXmlElement *actEl = h.FirstChild("Actions").FirstChild(action).Element();
    			if (!actEl){
    				// first make sure the Actions section exists
					if (!h.FirstChild("Actions").Element()){
						player->InsertEndChild(*(new TiXmlElement("Actions")));
					}
					actEl = new TiXmlElement(action);
					actEl->SetAttribute("key", key);
					h.FirstChild("Actions").Element()->InsertEndChild(*(actEl));
    			}
    			else {actEl->SetAttribute("key", key);}
    			return;
    		}
    	}
	    player=player->NextSiblingElement("Player");
    }
    // it appears that we did not find the player, must add it . .
    if (!lastPlayer) {
    	lastPlayer = docHandle.FirstChild("Balder").Element();
    }
    player = new TiXmlElement("Player");
    player->SetAttribute("controller", controller);
    player->InsertEndChild(*(new TiXmlElement("Actions")));
    player->FirstChildElement("Actions")->InsertEndChild(*(new TiXmlElement(action)));
    player->FirstChildElement("Actions")->FirstChildElement(action)->SetAttribute("key", key);
    docHandle.FirstChild("Balder").Element()->InsertAfterChild(lastPlayer, *player);
}

// Retrieves the color for player indicated in the 'controller' parameter from
// config file indicated in 'config_file' and returns it as type 'probe_color'.
// This functions will only load colors from the config file that are formatted
// in a 24 bit format.  Because of this, when you use ConfigManager::SetPlayerColor()
// you must make sure it is saving a color that is formatted in a 24 bit format.
// Please note that even though this function loads a 24 bit color, it'll reformat
// it so where it is a 32 bit color that'll include the value in 'cAlpha'.
const probe_color ConfigManager::GetPlayerColor(int controller, Uint8 cAlpha)
{
        Uint8 cRed;
        Uint8 cGreen;
        Uint8 cBlue;

	TiXmlHandle docHandle( config_file );
    TiXmlElement* key = docHandle.FirstChild( "Balder" ).FirstChild( "Player" ).Element();
    while(key)
    {
    	int c;
    	int result = key->QueryIntAttribute("controller",&c);
    	if (TIXML_SUCCESS == result){
    		if (controller == c){
    			TiXmlHandle h(key);
    			TiXmlElement *clrEl = h.FirstChild("Probe").FirstChild("Color").Element();
    			if (!clrEl) { return (probe_color)0;}
    			int colorSelected;
                        int gotColor = clrEl->QueryIntAttribute("probe_color", &colorSelected);
    			if (TIXML_SUCCESS == gotColor) {
                                if (colorSelected > 0xFFFFFF) colorSelected = 0xFFFFFF;
                                else if (colorSelected < 0) colorSelected = 0;

                                colorSelected = colorSelected << 8;
                                colorSelected += cAlpha;

                                return colorSelected;
                        }
    		}
    	}
	    key=key->NextSiblingElement("Player");
    }
    return 0;
}

// Sets the color for player indicated in the 'controller' parameter using
// the color indicated in the 'newColor' parameter to the saved player
// settings in the config file indicated by 'config_file'.
// The color in 'newColor' must be a color formatted in a 24 bit color format.
// Format for 'newColor' should equal the 24 bit format 'RRGGBB' because this
// functions mate, ConfigManager::GetPlayerColor() will only load a 24 bit color.
void ConfigManager::SetPlayerColor(int controller, probe_color newColor)
{
    if (newColor > 0xFFFFFF) newColor = 0xFFFFFF;
    else if (newColor < 0) newColor = 0;

    TiXmlHandle docHandle( config_file );
    TiXmlElement* player = docHandle.FirstChild( "Balder" ).FirstChild( "Player" ).Element();
    TiXmlElement* lastPlayer = 0;
    while(player)
    {
    	lastPlayer = player;
    	int c;
    	int result = player->QueryIntAttribute("controller",&c);
    	if (TIXML_SUCCESS == result){
    		if (controller == c){
    			TiXmlHandle h(player);
    			TiXmlElement *clrEl = h.FirstChild("Probe").FirstChild("Color").Element();
    			if (!clrEl){
    				// first make sure the Probe section exists
					if (!h.FirstChild("Probe").Element()){
						player->InsertEndChild(*(new TiXmlElement("Probe")));
					}
					clrEl = new TiXmlElement("Color");
					clrEl->SetAttribute("probe_color", newColor);
					h.FirstChild("Probe").Element()->InsertEndChild(*(clrEl));
    			}
    			else {clrEl->SetAttribute("probe_color", newColor);}
    			return;
    		}
    	}
	    player=player->NextSiblingElement("Player");
    }
    // it appears that we did not find the player, must add it . .
    if (!lastPlayer) {
    	lastPlayer = docHandle.FirstChild("Balder").Element();
    }
    player = new TiXmlElement("Player");
    player->SetAttribute("controller", controller);
    player->InsertEndChild(*(new TiXmlElement("Probe")));
    player->FirstChildElement("Probe")->InsertEndChild(*(new TiXmlElement("Color")));
    player->FirstChildElement("Probe")->FirstChildElement("Color")->SetAttribute("probe_color", newColor);
    docHandle.FirstChild("Balder").Element()->InsertAfterChild(lastPlayer, *player);
}

const int ConfigManager::GetNumPlayers()
{
	TiXmlHandle docHandle(config_file);
	TiXmlElement* numPlEl = docHandle.FirstChild( "Balder" )
			.FirstChild( "NumberOfPlayers" ).Element();
    if (!numPlEl) {return 0;}
    int n;
	int result = numPlEl->QueryIntAttribute("value",&n);
    if (TIXML_SUCCESS == result){ return n;}
    return 0;
}

void ConfigManager::SetNumPlayers(int n)
{
	TiXmlHandle docHandle(config_file);
	TiXmlElement* numPlEl = docHandle.FirstChild( "Balder" )
			.FirstChild( "NumberOfPlayers" ).Element();
	numPlEl->SetAttribute("value",n);
}

const std::string ConfigManager::GetControlType(int controller)
{
    TiXmlHandle docHandle( config_file );
    TiXmlElement* key = docHandle.FirstChild( "Balder" ).FirstChild( "Player" ).Element();
    while(key)
    {
    	int c;
    	int result = key->QueryIntAttribute("controller",&c);
    	if (TIXML_SUCCESS == result){
    		if (controller == c){
    			TiXmlHandle h(key);
    			TiXmlElement *controlEl = h.FirstChild("Control").Element();
    			if (!controlEl) { return "Human";} // human by default
    			std::string typestring = controlEl->Attribute("type");
    			return typestring;
    		}
    	}
	    key=key->NextSiblingElement("Player");
    }
    return "Human";
}

void ConfigManager::SetControlType(int controller, const char* type)
{
	TiXmlHandle docHandle( config_file );
    TiXmlElement* player = docHandle.FirstChild( "Balder" ).FirstChild( "Player" ).Element();
    TiXmlElement* lastPlayer = 0;
    while(player)
    {
    	lastPlayer = player;
    	int c;
    	int result = player->QueryIntAttribute("controller",&c);
    	if (TIXML_SUCCESS == result){
    		if (controller == c){
    			TiXmlHandle h(player);
    			TiXmlElement *controlEl = h.FirstChild("Control").Element();
    			if (!controlEl){
    				// first make sure the Control section exists
					controlEl = new TiXmlElement("Control");
					controlEl->SetAttribute("type", type);
    			}
    			else {controlEl->SetAttribute("type", type);}
    			return;
    		}
    	}
	    player=player->NextSiblingElement("Player");
    }
    // it appears that we did not find the player, must add it . .
    if (!lastPlayer) {
    	lastPlayer = docHandle.FirstChild("Balder").Element();
    }
    player = new TiXmlElement("Player");
    player->SetAttribute("controller", controller);
    player->InsertEndChild(*(new TiXmlElement("Control")));
    player->FirstChildElement("Control")->SetAttribute("type", type);
    docHandle.FirstChild("Balder").Element()->InsertAfterChild(lastPlayer, *player);
}

const char* ConfigManager::GetMap()
{
	TiXmlHandle docHandle(config_file);
	TiXmlElement* configElement = docHandle.FirstChild( "Balder" )
			.FirstChild( "Map" ).Element();
    if (!configElement) {error(); return 0;}
	const char* mapname = configElement->Attribute("name");
    return mapname;
}

void ConfigManager::SetMap(const char* mapname)
{
	TiXmlHandle docHandle(config_file);
	TiXmlElement* configElement = docHandle.FirstChild( "Balder" )
			.FirstChild( "Map" ).Element();
	configElement->SetAttribute("name", mapname);
}

const char* ConfigManager::GetServer()
{
	TiXmlHandle docHandle(config_file);
	TiXmlElement* configElement = docHandle.FirstChild( "Balder" )
			.FirstChild( "Server" ).Element();
	if (!configElement) {return "Enter Server Address";}
	const char* address = configElement->Attribute("address");
    return address;
}

void ConfigManager::SetServer(const char* address)
{
	TiXmlHandle docHandle(config_file);
	TiXmlElement* configElement = docHandle.FirstChild( "Balder" )
			.FirstChild( "Server" ).Element();
	if (!configElement){
		docHandle.FirstChild("Balder").Element()->
			InsertEndChild(*(new TiXmlElement("Server")));
		configElement = docHandle.FirstChild( "Balder" )
			.FirstChild( "Server" ).Element();
	}
	configElement->SetAttribute("address", address);
}

const int ConfigManager::GetIntValue(const char* section, const char* attribute)
{
	if (!config_file){LoadConfigFile();}
	TiXmlHandle docHandle(config_file);
	TiXmlElement* el = docHandle.FirstChild( "Balder" )
			.FirstChild( section ).FirstChild(attribute).Element();
	if (!el) {return 0;}
	int n;
	int result = el->QueryIntAttribute("value",&n);
    if (TIXML_SUCCESS == result){ return n;}
    return 0;
}

const double ConfigManager::GetDoubleValue(const char* section, const char* attribute)
{
	if (!config_file){LoadConfigFile();}
	TiXmlHandle docHandle(config_file);
	TiXmlElement* el = docHandle.FirstChild( "Balder" )
			.FirstChild( section ).FirstChild(attribute).Element();
	if (!el) {return 0;}
	double n;
	int result = el->QueryDoubleAttribute("value",&n);
    if (TIXML_SUCCESS == result){ return n;}
    return 0;
}

const char* ConfigManager::GetStringValue(const char* section, const char* attribute)
{
	if (!config_file){LoadConfigFile();}
	TiXmlHandle docHandle(config_file);
	TiXmlElement* el = docHandle.FirstChild( "Balder" )
			.FirstChild( section ).FirstChild(attribute).Element();
	if (!el) {return 0;}
	const char* string = el->Attribute("value");
    return string;
}

void ConfigManager::SetStringValue(const char* section, const char* attribute, const char* value)
{
	if (!config_file){LoadConfigFile();}
	// check carefully that the section and attribute nodes exist, if not create them
	TiXmlHandle docHandle(config_file);
	// assume at least the root "Balder" element exists
	TiXmlElement* el = docHandle.FirstChild( "Balder" ).FirstChild(section).Element();
	if (!el) {
		docHandle.FirstChild("Balder").Element()->
			InsertEndChild(*(new TiXmlElement(section)));
	}
	el = docHandle.FirstChild("Balder").FirstChild(section).FirstChild(attribute).Element();
	if (!el) {
		docHandle.FirstChild("Balder").FirstChild(section).Element()->
			InsertEndChild(*(new TiXmlElement(attribute)));
	}
	docHandle.FirstChild("Balder").FirstChild(section).FirstChild(attribute).
		Element()->SetAttribute("value", value);
}

void ConfigManager::SetIntValue(const char* section, const char* attribute, int value)
{
	if (!config_file){LoadConfigFile();}
	// check carefully that the section and attribute nodes exist, if not create them
	TiXmlHandle docHandle(config_file);
	// assume at least the root "Balder" element exists
	TiXmlElement* el = docHandle.FirstChild( "Balder" ).FirstChild(section).Element();
	if (!el) {
		docHandle.FirstChild("Balder").Element()->
			InsertEndChild(*(new TiXmlElement(section)));
	}
	el = docHandle.FirstChild("Balder").FirstChild(section).FirstChild(attribute).Element();
	if (!el) {
		docHandle.FirstChild("Balder").FirstChild(section).Element()->
			InsertEndChild(*(new TiXmlElement(attribute)));
	}
	docHandle.FirstChild("Balder").FirstChild(section).FirstChild(attribute).
		Element()->SetAttribute("value", value);
}

void ConfigManager::SetDoubleValue(const char* section, const char* attribute, double value)
{
	if (!config_file){LoadConfigFile();}
	// check carefully that the section and attribute nodes exist, if not create them
	TiXmlHandle docHandle(config_file);
	// assume at least the root "Balder" element exists
	TiXmlElement* el = docHandle.FirstChild( "Balder" ).FirstChild(section).Element();
	if (!el) {
		docHandle.FirstChild("Balder").Element()->
			InsertEndChild(*(new TiXmlElement(section)));
	}
	el = docHandle.FirstChild("Balder").FirstChild(section).FirstChild(attribute).Element();
	if (!el) {
		docHandle.FirstChild("Balder").FirstChild(section).Element()->
			InsertEndChild(*(new TiXmlElement(attribute)));
	}
	docHandle.FirstChild("Balder").FirstChild(section).FirstChild(attribute).
		Element()->SetDoubleAttribute("value", value);
}

void ConfigManager::GenerateConfig( const char *filename)
{
    std::ofstream cfile( filename );
    cfile
        << "<?xml version=\"1.0\" ?> "
        << "<!-- Balder2d Configuration data --> "
        << "<Balder> "
        << "    <!-- The following is meant to be modified by the user --> "
        << "    <Map name=\"tri-duel\" /> "
        << "    <NumberOfPlayers value=\"3\" /> "
        << "    <GameSetup> "
        << "        <!-- negative values indicate disabled modes --> "
        << "        <ScoreLimit value=\"100\" /> "
        << "        <!-- time limit is in minutes --> "
        << "        <TimeLimit value=\"8\" /> "
        << "        <LifeLimit value=\"15\" /> "
        << "    </GameSetup> "
        << "    <Video> "
        << "        <fullscreen value=\"0\" /> "
        << "    </Video> "
        << "    <Sound> "
        << "        <fxvolume value=\"0.5\" /> "
        << "        <musicvolume value=\"0.5\" /> "
        << "        <music value=\"1\" /> "
        << "        <sound value=\"1\" /> "
        << "    </Sound> "
        << "    <Player controller=\"1\"> "
        << "        <Control type=\"Human\" /> "
        << "        <Actions> "
        << "            <right key=\"RIGHT\" /> "
        << "            <left key=\"LEFT\" /> "
        << "            <stick key=\"DOWN\" /> "
        << "            <push key=\"UP\" /> "
        << "            <fire key=\"UP\" /> "
        << "        </Actions> "
        << "        <Probe> "
        << "            <Color probe_color=\"8487423\" /> "
        << "        </Probe> "
        << "    </Player> "
        << "    <Player controller=\"2\"> "
        << "        <Control type=\"balderai\" /> "
        << "        <Probe> "
        << "            <Color probe_color=\"16711680\" /> "
        << "        </Probe> "
        << "    </Player> "
        << "    <Player controller=\"3\"> "
        << "        <Control type=\"balderai\" /> "
        << "        <Probe> "
        << "           <Color probe_color=\"65280\" /> "
        << "        </Probe> "
        << "    </Player> "
        << "    <!-- Values below this line are for tweaking different aspects of the game. "
        << "         regular users will not usually want to mess with this stuff. --> "
        << "    <Entity> "
        << "        <maxvelocity value=\"5\" /> "
        << "    </Entity> "
        << "    <Probe> "
        << "        <maxhits value=\"3\" /> "
        << "        <maxcharge value=\"5\" /> "
        << "        <rechargetime value=\"400\" /> "
        << "        <maxspeed value=\"3\" /> "
        << "        <poweruptime value=\"10\" /> "
        << "        <turningspeed value=\"0.041666667\" /> "
        << "    </Probe> "
        << "    <Projectile> "
        << "        <radius value=\"8\" /> "
        << "        <lifetime value=\"200\" /> "
        << "        <frames value=\"8\" /> "
        << "        <explodeframes value=\"8\" /> "
        << "        <exploderadius value=\"16\" /> "
        << "        <speed value=\"4\" />"
        << "        <AddProbeVelocity value=\"0\" /> "
        << "    </Projectile> "
        << "    <Powerup> "
        << "        <spawn_interval value=\"400\" /> "
        << "    </Powerup> "
        << "</Balder> "
        << std::endl ;
    cfile.close();
}
