/***************************************************************************
 *   Copyright (C) 2008 by Bjorn Hansen                                    *
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
#ifndef SKINNEDDROPDOWN_H
#define SKINNEDDROPDOWN_H
#include <guichan/widgets/dropdown.hpp>
#include <guichan/widgets/listbox.hpp>
#include <guichan/widgets/scrollarea.hpp>

namespace Balder
{
    /**
     * a better looking listbox
     */
    class SkinnedListBox: public gcn::ListBox
    {
    public:
        // Inherited from gcn::Widget
        virtual void draw(gcn::Graphics* graphics);
    };

    /**
     * a better looking dropdown
     */
    class SkinnedDropDown: public gcn::DropDown
    {
    public:
        SkinnedDropDown(gcn::ListModel *listModel, gcn::ScrollArea *scrollArea, gcn::ListBox *listBox):
          gcn::DropDown(listModel, scrollArea, listBox) {}
        // Inherited from gcn::Widget
        virtual void keyPressed(gcn::KeyEvent& keyEvent);
        virtual void draw(gcn::Graphics* graphics);
        virtual void drawButton(gcn::Graphics* graphics);
    };

     /**
     * a better looking scroll bar
     */
    class SkinnedScrollArea: public gcn::ScrollArea
    {
    public:
        SkinnedScrollArea(): gcn::ScrollArea() {}
        // Inherited from gcn::Widget
        virtual void draw(gcn::Graphics* graphics);
        virtual void drawVBar(gcn::Graphics* graphics);
        virtual void drawVMarker(gcn::Graphics* graphics);
    };
}

#endif // SKINNEDDROPDOWN_H
