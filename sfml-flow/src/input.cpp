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

#include "input.h"

namespace flow
{

Input::Input()
{
    sf::Text* label = new sf::Text();
    label->setFont(getFont());
    label->setColor(sf::Color::White);
    label->setCharacterSize(12);
    label->setPosition(sf::Vector2f(-label->getLocalBounds().width/2,
                                    -label->getLocalBounds().height*.7));
    addDrawer("label", label);
}

bool Input::onText(sf::Uint32 unicode)
{
    if(unicode == 8
            || unicode == 13
            || unicode == 27) return false;

    sf::String s;
    s.insert(0, unicode);
    std::wstring ws = s.toWideString();
    input_.insert(input_.getSize(), unicode);
    render<sf::Text>("label").setString(input_);
    return true;
}

bool Input::onKey(sf::Keyboard::Key key)
{
    if(key == sf::Keyboard::BackSpace)
    {
        if(input_.getSize() > 0)
        {
            input_.erase(input_.getSize() - 1);
            render<sf::Text>("label").setString(input_);
        }
        return true;
    }
    return false;
}

void Input::reset()
{
    input_.clear();
    render<sf::Text>("label").setString("");
}

std::string Input::getText() const
{
    return input_.toAnsiString();
}

void Input::setText(const std::string& text)
{
    input_ = text;
    render<sf::Text>("label").setString(input_);
}

}
