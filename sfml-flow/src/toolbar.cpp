/////////////////////////////////////////////////////////////////////////
/// sfml-flow
/// Copyright (C) 2014 Jérôme Béchu
///
/// This program is free software: you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
///
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
///
/// You should have received a copy of the GNU General Public License
/// along with this program.  If not, see <http://www.gnu.org/licenses/>.
/////////////////////////////////////////////////////////////////////////

#include "toolbar.h"

namespace flow
{

Toolbar::Toolbar()
    : offset_(0)
{
}

Widget::Ptr Toolbar::add(const std::string& label,
                         boost::function<void()> callback,
                         sf::Uint32 shortcut)
{
    Button::Ptr b = addChild<Button>(
                boost::make_shared<Button>(sf::Vector2f(kButtonSize, 30), label, callback));
    b->setPosition(sf::Vector2f(offset_, 0));
    offset_+=kOffsetSize;
    shortcuts_[shortcut] = b;
    return b;
}

bool Toolbar::onText(sf::Uint32 unicode)
{
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
    {
        if(shortcuts_.find(unicode) != shortcuts_.end())
        {
            shortcuts_[unicode]->call();
            return true;
        }
        std::cout<<"ctrl+"<<unicode<<std::endl;
    }
    return Widget::onText(unicode);
}

}
