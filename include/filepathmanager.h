/***************************************************************************
 *   Copyright (C) 2007 by Bjorn Hansen                                    *
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

#ifndef FilePathManager_H
#define FilePathManager_H

#include <string>
#include <list>

namespace Balder {

class FilePathManager
{
    public:
        static void Init(int argc, char *argv[]);
        static void Cleanup();
        static std::string GetFullPath(const char* filename);
        static std::list<std::string> GetSearchPath();
        static std::string GetConfigPath(){return configPath;}
    protected:
        static bool ParseArgs(int argc, char *argv[], const char* flag, std::string *value);
        static bool SetConfig(const char* data_directory);
    private:
        FilePathManager(){};
        ~FilePathManager(){};
        static std::string configPath;
};

}

#endif // FilePathManager_H
