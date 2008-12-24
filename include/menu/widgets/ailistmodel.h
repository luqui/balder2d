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

#ifndef AILISTMODEL_H
#define AILISTMODEL_H

#include <guichan/listmodel.hpp>
#include <vector>

namespace Balder {

class AIListModel : public gcn::ListModel{
public:

	AIListModel();
	virtual ~AIListModel();
	virtual int getNumberOfElements () {return numScripts;}
	virtual std::string getElementAt (int i) {return scriptnames[i];}
    int FindScriptIndex(std::string scriptname);
private:
	void FindScripts();
	int numScripts;
	std::vector<std::string> scriptnames;
};
}
#endif // AILISTMODEL_H
