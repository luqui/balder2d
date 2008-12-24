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

#include <sstream>
#include <guichan/font.hpp>
#include <guichan/graphics.hpp>
#include "menu/widgets/numberfield.h"
#include "balder2dtypes.h"
#include "log.h"

using namespace Balder;
using namespace gcn;

NumberField::NumberField(int n,int maxDigits): digits(maxDigits),gcn::TextField()
{
  std::stringstream ss;
  ss << n;
	setText(ss.str());
	setWidth(getFont()->getWidth("0")*maxDigits + 4);
	adjustHeight();
}

int NumberField::getValue()
{
	if (mText.size() < 1) return 0;
	std::istringstream ss(mText);
	int value;
	ss >> value;
	return value;
}

void NumberField::setValue(int n)
{
	std::stringstream ss;
  	ss << n;
	setText(ss.str());
}

void NumberField::mousePressed(MouseEvent& e)
{
	if (e.getButton() == MouseEvent::LEFT)
	{
		mCaretPosition = getFont()->getStringIndexAt(mText, e.getX() + mXScroll);
		fixScroll();
	}
}

void NumberField::keyPressed(gcn::KeyEvent& keyEvent)
{
  gcn::Key key = keyEvent.getKey();
	if (key.getValue() == Key::LEFT && mCaretPosition > 0)
    {
    	--mCaretPosition;
	}

	else if (key.getValue() == Key::RIGHT && mCaretPosition < mText.size())
	{
		++mCaretPosition;
	}

	else if (key.getValue() == Key::DELETE && mCaretPosition < mText.size())
	{
		mText.erase(mCaretPosition, 1);
	}

	else if (key.getValue() == Key::BACKSPACE && mCaretPosition > 0)
	{
		mText.erase(mCaretPosition - 1, 1);
		--mCaretPosition;
	}

	else if (key.getValue() == Key::ENTER)
	{
		distributeActionEvent();
	}

	else if (key.getValue() == Key::HOME)
	{
		mCaretPosition = 0;
	}

	else if (key.getValue() == Key::END)
	{
		mCaretPosition = mText.size();
	}

	else if (key.isNumber())
	{
		if (mText.size() < digits){
			mText.insert(mCaretPosition, std::string(1,(char)key.getValue()));
			++mCaretPosition;
		}
	}
}

void NumberField::draw(Graphics* graphics)
    {
      Color highlight = MenuColors::HIGHLIGHT;
      Color shadow = MenuColors::SHADOW;
        if (isFocused())
        {
          Color bgColor = MenuColors::FOCUSED;
          bgColor.a = MenuColors::bgAlpha;
          graphics->setColor(bgColor);
          graphics->fillRectangle(Rectangle(0, 0, getWidth(), getHeight()));
          drawCaret(graphics, getWidth()-getFont()->getWidth(mText.substr(mCaretPosition,mText.size())) - 4);
        }
        // draw the border
        for (int i=0; i<5 ;++i)
        {
          shadow.a -= i*0x10;
          graphics->setColor(shadow);
          graphics->drawLine(i,i,getWidth()-i,i);
          graphics->drawLine(i,i,i,getHeight()-i);
        }
        for (int i=0; i<5 ;++i)
        {
          highlight.a -= i*0x10;
          graphics->setColor(highlight);
          graphics->drawLine(getWidth()-i,i,getWidth()-i,getHeight()-i);
          graphics->drawLine(i,getHeight()-i,getWidth()-i,getHeight()-i);
        }

        graphics->setColor(getForegroundColor());
        graphics->setFont(getFont());
        //graphics->drawText(mText, 1 - mXScroll, 1);
        graphics->drawText(mText,getWidth()-1, 10, gcn::Graphics::RIGHT);
    }
