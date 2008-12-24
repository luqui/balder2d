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

#ifndef BALDERNUMBERFIELD_H
#define BALDERNUMBERFIELD_H

#include <guichan/widgets/textfield.hpp>

namespace Balder {

class NumberField : public gcn::TextField
{
public:
    NumberField(int n, int maxDigits = 3);
    virtual void mousePressed(gcn::MouseEvent &e);
    virtual void keyPressed(gcn::KeyEvent& keyEvent);
    virtual void draw(gcn::Graphics*);
    int getValue();
    void setValue(int n);
private:
	int digits;
};

}

#endif
