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

#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include "balder2dtypes.h"
#include <string>

class TiXmlDocument;
class TiXmlElement;
class TiXmlComment;
class TiXmlUnknown;
class TiXmlAttribute;
class TiXmlText;
class TiXmlDeclaration;
class TiXmlParsingData;

namespace Balder{

class ConfigManager{
public:
	/// loads the default config file if no config file is loaded
	static void LoadConfigFile();
	static void SaveConfigFile();
	/// loads an arbitrary config file
	static void LoadConfigFile(const char* filename);
	/// find the keybinding in the config file for the action and controller #
	static const char* GetPlayerControl(int controller, const char* action);
	static void SetPlayerControl(int controller, const char* action, const char* key);
        // Retrieves the color for player indicated in the 'controller' parameter from
        // config file indicated in 'config_file' and returns it as type 'probe_color'.
	static const probe_color GetPlayerColor(int controller, Uint8 cAlpha);
        // Sets the color for player indicated in the 'controller' parameter using
        // the color indicated in the 'newColor' parameter to the saved player
        // settings in the config file indicated by 'config_file'.
	static void SetPlayerColor(int controller, probe_color newColor);
	static const int GetNumPlayers();
	static void SetNumPlayers(int num);
    static const std::string GetControlType(int controller);
    static void SetControlType(int controller, const char* type);
	static const char* GetMap();
	static void SetMap(const char* name);
	static const char* GetServer();
	static void SetServer(const char* address);
	static const int GetIntValue(const char* section, const char* attribute);
	static const double GetDoubleValue(const char* section, const char* attribute);
	static const char* GetStringValue(const char* section, const char* attribute);
	static void SetIntValue(const char* section, const char* attribute, int value);
	static void SetDoubleValue(const char* section, const char* attribute, double value);
	static void SetStringValue(const char* section, const char* attribute, const char* value);
private:
	static TiXmlDocument *config_file;
    static void GenerateConfig( const char *filename );
};
}

#endif // CONFIGMANAGER_H
