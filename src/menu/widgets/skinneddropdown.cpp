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

#include <sstream>
#include <guichan/font.hpp>
#include <guichan/graphics.hpp>
#include "menu/widgets/skinneddropdown.h"
#include "balder2dtypes.h"

using namespace Balder;
using namespace gcn;

void SkinnedListBox::draw(Graphics* graphics)
{
    Color bgColor = MenuColors::UNFOCUSED;
    //bgColor.a = MenuColors::bgAlpha;
    graphics->setColor(bgColor);
    graphics->fillRectangle(Rectangle(0, 0, getWidth(), getHeight()));

    if (mListModel == NULL)
    {
        return;
    }

    graphics->setColor(getForegroundColor());
    graphics->setFont(getFont());

    // Check the current clip area so we don't draw unnecessary items
    // that are not visible.
    const ClipRectangle currentClipArea = graphics->getCurrentClipArea();
    int rowHeight = getRowHeight();

    // Calculate the number of rows to draw by checking the clip area.
    // The addition of two makes covers a partial visible row at the top
    // and a partial visible row at the bottom.
    int numberOfRows = currentClipArea.height / rowHeight + 2;

    if (numberOfRows > mListModel->getNumberOfElements())
    {
        numberOfRows = mListModel->getNumberOfElements();
    }

    // Calculate which row to start drawing. If the list box
    // has a negative y coordinate value we should check if
    // we should drop rows in the begining of the list as
    // they might not be visible. A negative y value is very
    // common if the list box for instance resides in a scroll
    // area and the user has scrolled the list box downwards.
    int startRow;
    if (getY() < 0)
    {
        startRow = -1 * (getY() / rowHeight);
    }
    else
    {
        startRow = 0;
    }

    // end row should not exceed list size
    int endRow = std::min(startRow+numberOfRows, mListModel->getNumberOfElements());
    // The y coordinate where we start to draw the text is
    // simply the y coordinate multiplied with the font height.
    int y = rowHeight * startRow;
    for (int i = startRow; i < endRow; ++i)
    {
        if (i == mSelected)
        {
            graphics->setColor(getSelectionColor());
            graphics->fillRectangle(Rectangle(0, y, getWidth(), rowHeight));
            graphics->setColor(getForegroundColor());
        }

        // If the row height is greater than the font height we
        // draw the text with a center vertical alignment.
        if (rowHeight > getFont()->getHeight())
        {
            graphics->drawText(mListModel->getElementAt(i), 1, y + rowHeight / 2 - getFont()->getHeight() / 2);
        }
        else
        {
            graphics->drawText(mListModel->getElementAt(i), 1, y);
        }

        y += rowHeight;
    }
}

void SkinnedDropDown::keyPressed(KeyEvent& keyEvent)
{
    // work around guichan bug where when the dropdown is open arrow keys get counted twice (skips elements)
    if (keyEvent.isConsumed()) return;
    gcn::DropDown::keyPressed(keyEvent);
}

void SkinnedDropDown::draw(Graphics* graphics)
{
    int h;

    if (mDroppedDown)
    {
        h = mFoldedUpHeight;
    }
    else
    {
        h = getHeight();
    }

    Color highlightColor, shadowColor;
    highlightColor = MenuColors::HIGHLIGHT;
    shadowColor = MenuColors::SHADOW;

    // draw the border
    for (int i=0; i<5 ;++i)
    {
        shadowColor.a -= i*0x10;
        graphics->setColor(shadowColor);
        graphics->drawLine(i,i,getWidth()-i,i);
        graphics->drawLine(i,i,i,getHeight()-i);
    }
    for (int i=0; i<5 ;++i)
    {
        highlightColor.a -= i*0x10;
        graphics->setColor(highlightColor);
        graphics->drawLine(getWidth()-i,i,getWidth()-i,getHeight()-i);
        graphics->drawLine(i,getHeight()-i,getWidth()-i,getHeight()-i);
    }

    // Push a clip area so the other drawings don't need to worry
    // about the border.
    graphics->pushClipArea(Rectangle(1, 1, getWidth() - 2, h - 2));
    const Rectangle currentClipArea = graphics->getCurrentClipArea();

    Color bgColor = isFocused() ? MenuColors::FOCUSED : MenuColors::UNFOCUSED;
    bgColor.a = MenuColors::bgAlpha;
    graphics->setColor(bgColor);
    graphics->fillRectangle(Rectangle(0, 0, currentClipArea.width, currentClipArea.height));

    if (mListBox->getListModel()
            && mListBox->getSelected() >= 0)
    {
        graphics->setColor(getForegroundColor());
        graphics->setFont(getFont());

        graphics->drawText(mListBox->getListModel()->getElementAt(mListBox->getSelected()), 1, 5);
    }

    // Push a clip area before drawing the button.
    graphics->pushClipArea(Rectangle(currentClipArea.width - currentClipArea.height,
                                     0,
                                     currentClipArea.height,
                                     currentClipArea.height));
    drawButton(graphics);
    graphics->popClipArea();
    graphics->popClipArea();

    if (mDroppedDown)
    {
        // Draw a border around the children.
//        graphics->setColor(shadowColor);
//        graphics->drawRectangle(Rectangle(0,
//                                          mFoldedUpHeight,
//                                          getWidth(),
//                                          getHeight() - mFoldedUpHeight));
        drawChildren(graphics);
    }
}

void SkinnedDropDown::drawButton(Graphics *graphics)
{
    Color faceColor, highlightColor, shadowColor;
    int offset;
    int alpha = MenuColors::bgAlpha;

    if (mPushed)
    {
        faceColor = MenuColors::FOCUSED;
        faceColor.a = alpha;
        highlightColor = MenuColors::SHADOW;
        highlightColor.a = alpha;
        shadowColor = MenuColors::HIGHLIGHT;
        shadowColor.a = alpha;
        offset = 1;
    }
    else
    {
        faceColor = MenuColors::FOCUSED;
        faceColor.a = alpha;
        highlightColor = MenuColors::HIGHLIGHT;
        highlightColor.a = alpha;
        shadowColor = MenuColors::SHADOW;
        shadowColor.a = alpha;
        offset = 0;
    }

    const Rectangle currentClipArea = graphics->getCurrentClipArea();
    for (int i = 0; i<3; ++i)
    {
        graphics->setColor(highlightColor);
        graphics->drawLine(i, i, currentClipArea.width - i, i);
        graphics->drawLine(i, i, i, currentClipArea.height - i);
        graphics->setColor(shadowColor);
        graphics->drawLine(currentClipArea.width - i, i, currentClipArea.width - i,currentClipArea.height - i);
        graphics->drawLine(i, currentClipArea.height - i, currentClipArea.width - i,currentClipArea.height - i);
    }

    graphics->setColor(faceColor);
    graphics->fillRectangle(Rectangle(1,
                                      1,
                                      currentClipArea.width - 2,
                                      currentClipArea.height - 2));

    graphics->setColor(MenuColors::fontColor);

    int i;
    int n = currentClipArea.height / 3;
    int dx = currentClipArea.height / 2;
    int dy = (currentClipArea.height * 2) / 3;
    for (i = 0; i < n; i++)
    {
        graphics->drawLine(dx - i + offset,
                           dy - i + offset,
                           dx + i + offset,
                           dy - i + offset);
    }
}

void SkinnedScrollArea::draw(Graphics *graphics)
{
        drawBackground(graphics);

        if (mVBarVisible)
        {
//            drawUpButton(graphics);
//            drawDownButton(graphics);
            drawVBar(graphics);
            drawVMarker(graphics);

        }

    drawChildren(graphics);
}

void SkinnedScrollArea::drawVBar(Graphics* graphics)
{
    Rectangle dim(getWidth() - mScrollbarWidth, 0, mScrollbarWidth, getHeight());


    graphics->pushClipArea(dim);

    int alpha = MenuColors::bgAlpha;
    Color trackColor = MenuColors::UNFOCUSED;
    trackColor.a = alpha;
    Color shadowColor = MenuColors::SHADOW;
    shadowColor.a = alpha;

    graphics->setColor(trackColor);
    graphics->fillRectangle(Rectangle(0, 0, dim.width, dim.height));

    graphics->setColor(shadowColor);
    graphics->drawLine(0, 0, 0, dim.height);

    graphics->popClipArea();
}

void SkinnedScrollArea::drawVMarker(Graphics* graphics)
{
    Rectangle dim = getVerticalMarkerDimension();
    graphics->pushClipArea(dim);

    int alpha = MenuColors::bgAlpha;
    Color faceColor = MenuColors::fontColor;
    Color highlightColor = MenuColors::HIGHLIGHT;
    highlightColor.a = alpha;
    Color shadowColor = MenuColors::SHADOW;
    shadowColor.a = alpha;

    graphics->setColor(faceColor);
    graphics->fillRectangle(Rectangle(1, 1, dim.width - 1, dim.height - 1));

    graphics->setColor(highlightColor);
    graphics->drawLine(0, 0, dim.width - 1, 0);
    graphics->drawLine(0, 1, 0, dim.height - 1);

    graphics->setColor(shadowColor);
    graphics->drawLine(1, dim.height - 1, dim.width - 1, dim.height - 1);
    graphics->drawLine(dim.width - 1, 0, dim.width - 1, dim.height - 1);

    graphics->popClipArea();
}
