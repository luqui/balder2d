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

#include "filepathmanager.h"
#include <physfs.h>
#include "log.h"

using namespace Balder;
using namespace std;

string FilePathManager::configPath = "config.xml";

void FilePathManager::Init(int argc, char *argv[])
{
    PHYSFS_init(argv[0]);
    string s;
    if (ParseArgs(argc, argv, "config", &s)) {
        configPath = s;
    }
    if (ParseArgs(argc, argv, "data", &s)){
        SetConfig(s.c_str());
    }
    else {
        SetConfig(NULL);
    }
    char **searchpath = PHYSFS_getSearchPath();
    char **i;
    Log::output("search path set to:");
    for ( i=searchpath; *i != NULL; ++i){
        Log::output(*i);
    }
    PHYSFS_freeList(searchpath);
}

void FilePathManager::Cleanup()
{
    PHYSFS_deinit();
}
// returns true if the flag is found and the value for that flag is assigned to *value
bool FilePathManager::ParseArgs(int argc, char *argv[], const char* flag, string *value)
{
    string flagstring = "--";
    flagstring += flag;
    for (int i=1;i<argc-1;++i){
        if (argv[i] == flagstring){
            *value = argv[i+1];
            return true;
        }
    }
    return false;
}

bool FilePathManager::SetConfig(const char* data_directory)
{
    const char *basedir = PHYSFS_getBaseDir();
    const char *userdir = PHYSFS_getUserDir();
    const char *dirsep = PHYSFS_getDirSeparator();
    const char *dir = "balder2d";

    string str(userdir);
    str.append(dir);

    if (!PHYSFS_setWriteDir(str.c_str()))
    {
        cout << "setWriteDir failed" << endl;
        int no_write = 0;
        if ( (PHYSFS_setWriteDir(userdir)) &&
             (PHYSFS_mkdir(dir)) )
        {
            cout << "set write directory and created " << dir << " directory" <<endl;
            if (!PHYSFS_setWriteDir(str.c_str())){
                cout << "failed to set write directory to " << dir << endl;
                no_write = 1;
            }
        } /* if */
        else
        {
                no_write = 1;
        } /* else */

        if (no_write)
        {
            cout << "setting write directory to null " << endl;
            PHYSFS_setWriteDir(NULL);   /* just in case. */
        } /* if */
    } /* if */

    /* Put write dir first in search path... */
    PHYSFS_addToSearchPath(str.c_str(), 0);
    // add the data directory if it was specified
    if (data_directory != NULL){ PHYSFS_addToSearchPath(data_directory,1);}
    /* Put base path on search path... */
    PHYSFS_addToSearchPath(basedir, 1);

    return(true);
}

string FilePathManager::GetFullPath(const char* filename)
{
    const char* path = PHYSFS_getRealDir(filename);
    if (NULL == path) {
        Log::output(std::string("file not found: ").append(filename));
        return "";
    }
    std::string fullpath(path);
    fullpath.append(PHYSFS_getDirSeparator()).append(filename);
    return fullpath;
}

std::list<std::string> FilePathManager::GetSearchPath()
{
    std::list<std::string> searchpath;
    char **sp = PHYSFS_getSearchPath();
    char **i;
    for (i = sp; *i != NULL; i++){
        searchpath.push_back(std::string(*i));
    }
    PHYSFS_freeList(sp);
    return searchpath;
}
