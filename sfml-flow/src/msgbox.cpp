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

#include "msgbox.h"
#include "button.h"

namespace flow
{

MsgBox::MsgBox(const sf::Vector2f &size)
    : Widget(size)
{
    sf::RectangleShape* bg = new sf::RectangleShape(size);
    bg->setFillColor(sf::Color(10, 10, 10, 200));
    bg->setOrigin(sf::Vector2f(size.x/2, size.y/2));
    addDrawer("bg", bg);

    sf::Text* label = new sf::Text();
    label->setFont(getFont());
    label->setColor(sf::Color::Red);
    label->setCharacterSize(20);
    addDrawer("label", label);

    Button::Ptr ok = addChild<Button>(boost::make_shared<Button>(sf::Vector2f(100, 30),
                                          "OK",
                                          boost::bind(&Widget::hide, this)));
    ok->setPosition(sf::Vector2f(0, 100));
}

void MsgBox::setValue(const std::string& value)
{
    render<sf::Text>("label").setString(value);
    render<sf::Text>("label").setPosition(sf::Vector2f(-render<sf::Text>("label").getLocalBounds().width/2,
                                                       -render<sf::Text>("label").getLocalBounds().height*.7));
}

}
