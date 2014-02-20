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

#include "tooltip.h"

namespace flow
{

Tooltip::Tooltip()
    : Widget(sf::Vector2f(100, 15))
{
    sf::RectangleShape* bg = new sf::RectangleShape(size_);
    bg->setOrigin(sf::Vector2f(size_.x/2, size_.y/2));
    bg->setFillColor(sf::Color::Black);
    addDrawer("bg", bg);

    sf::Text* label = new sf::Text();
    label->setFont(getFont());
    label->setColor(sf::Color::White);
    label->setCharacterSize(12);
    label->setPosition(sf::Vector2f(-label->getLocalBounds().width/2,
                                    -label->getLocalBounds().height*.7));
    addDrawer("label", label);
    hide();
}

bool Tooltip::onText(sf::Uint32 unicode)
{
    if(activated_ == false) return false;
    if(unicode == 8
            || unicode == 13
            || unicode == 27) return false;

    sf::String s;
    s.insert(0, unicode);
    //std::wstring ws = s.toWideString();
    input_.insert(input_.getSize(), unicode);
    render<sf::Text>("label").setString(input_);
    render<sf::Text>("label").setPosition(sf::Vector2f(-render<sf::Text>("label").getLocalBounds().width/2,
                                                       -render<sf::Text>("label").getLocalBounds().height*.7));
    return true;
}

bool Tooltip::onKey(sf::Keyboard::Key key)
{
    if(activated_ == false) return false;
    if(key == sf::Keyboard::BackSpace)
    {
        if(input_.getSize() > 0)
        {
            input_.erase(input_.getSize() - 1);
            render<sf::Text>("label").setString(input_);
            render<sf::Text>("label").setPosition(sf::Vector2f(-render<sf::Text>("label").getLocalBounds().width/2,
                                                               -render<sf::Text>("label").getLocalBounds().height*.7));
        }
        return true;
    }
    return false;
}

void Tooltip::activate()
{
    activated_ = true;
}

void Tooltip::deactivate()
{
    activated_ = false;
}

void Tooltip::setValue(const std::string& value)
{
    input_ = value;
    render<sf::Text>("label").setString(value);
    render<sf::Text>("label").setPosition(sf::Vector2f(-render<sf::Text>("label").getLocalBounds().width/2,
                                                       -render<sf::Text>("label").getLocalBounds().height*.7));
}

std::string Tooltip::getValue() const
{
    return input_.toAnsiString();
}

}
