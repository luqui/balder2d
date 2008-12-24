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

#include "menu/widgets/ailistmodel.h"
#include <physfs.h>

using namespace Balder;

AIListModel::AIListModel(): numScripts(0)
{
	FindScripts();
}
AIListModel::~AIListModel()
{}

void AIListModel::FindScripts()
{
    std::string script_dir("scripts/");
    // always show Human option as it disables any ai scripting so that a user may play.
	scriptnames.push_back("Human");
    ++numScripts;
    char **files = PHYSFS_enumerateFiles("scripts/");
    char **i;
    std::string script;
    for (i = files; *i != NULL; i++){
        script = *i;
        if (!PHYSFS_isDirectory((script_dir+script).c_str()) && !script.compare(script.size()-3, 3, ".py")){
            scriptnames.push_back(script.substr(0,script.size()-3));
            ++numScripts;
        }
    }
    PHYSFS_freeList(files);
}


/*!
    \fn Balder::AIListModel::FindScriptIndex(std::string scriptname)
 */
int Balder::AIListModel::FindScriptIndex(std::string scriptname)
{
    for (int i = 0; i < numScripts; ++i) {
    	if (!scriptname.compare(scriptnames[i])) return i;
    }
    return 0;
}
