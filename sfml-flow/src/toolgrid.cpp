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

#include "toolgrid.h"

namespace flow
{

Toolgrid::Toolgrid(const sf::Vector2f &size)
    : Widget(size)
    , offset_x_(0)
    , offset_y_(0)
    , column_(5)
{
    sf::RectangleShape* bg = new sf::RectangleShape(size);
    bg->setFillColor(sf::Color(10, 10, 10, 200));
    addDrawer("bg", bg);
}

void Toolgrid::add(const std::string& label,
               boost::function<void(const std::string&)> callback)
{
    std::string copy = label;
    while(copy.size() > kMaxCharacters)
    {
        copy.erase(copy.begin()+copy.size()/2);
    }
    if(copy.size() != label.size())
        copy.insert(copy.begin()+copy.size()/2, '*');
    Button::Ptr b = addChild<Button>(
                boost::make_shared<Button>(sf::Vector2f(100, 15), copy, callback, label));
    b->setPosition(sf::Vector2f(60+offset_x_*110, 30 + offset_y_*40));
    offset_x_++;
    if(offset_x_ >= column_)
    {
        offset_x_ = 0;
        offset_y_++;
    }
}

void Toolgrid::clear()
{
    offset_x_ = 0;
    offset_y_ = 0;
    Widget::clear();
}

}
