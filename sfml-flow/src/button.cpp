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

#include "button.h"

namespace flow
{

Button::Button(const sf::Vector2f& size,
               const std::string& name,
               boost::function<void()> callback,
               Shape shape)
    : Widget(size)
    , callback_(callback)
    , pushed_(false)
    , node_(0)
{
    setup(name, shape);
}

Button::Button(const sf::Vector2f& size,
               const std::string& name,
               Shape shape)
    : Widget(size)
    , pushed_(false)
    , node_(0)
{
    setup(name, shape);
}

Button::Button(const sf::Vector2f& size,
               const std::string& name,
               boost::function<void(const std::string &)> callback,
               const std::string& value,
               Shape shape)
    : Widget(size)
    , value_(value)
    , callback2_(callback)
    , node_(0)
{
    setup(name, shape);
}

Button::Button(const sf::Vector2f& size,
               const std::string& name,
               boost::function<void(Node*)> callback,
               Node* node,
               Shape shape)
    : Widget(size)
    , callback3_(callback)
    , node_(node)
{
    setup(name, shape);
}

std::string Button::getLabel() const
{
    return name_;
}

void Button::onMouseOver()
{
    render<sf::RectangleShape>("bg").setFillColor(sf::Color(120, 40, 200));
}

void Button::onMouseOut()
{
    render<sf::RectangleShape>("bg").setFillColor(sf::Color(200, 200, 200));
}

void Button::onMouseLeftPressed()
{
    pushed_ = true;
}

bool Button::onMouseLeftReleased()
{
    if(pushed_)
    {
        call();
        return true;
    }
    pushed_ = false;
    return false;
}

void Button::call()
{
    if(value_.size() > 0)
        callback2_(value_);
    else if(node_)
        callback3_(node_);
    else
        callback_();
}

void Button::setCallback(boost::function<void(const std::string&)> callback, const std::string& value)
{
    value_ = value;
    callback2_ = callback;
}

void Button::setCallback(boost::function<void()> callback)
{
    value_.clear();
    callback_ = callback;
}

void Button::setup(const std::string& name, Shape shape)
{
    name_ = name;

    if(shape == kRectangle)
    {
        sf::RectangleShape* bg = new sf::RectangleShape(size_);
        bg->setOrigin(sf::Vector2f(size_.x/2, size_.y/2));
        addDrawer("bg", bg);
    }
    else
    {
        sf::CircleShape* bg = new sf::CircleShape(size_.x);
        bg->setOrigin(sf::Vector2f(size_.x, size_.x));
        addDrawer("bg", bg);
    }

    sf::Text* label = new sf::Text();
    label->setFont(getFont());
    label->setColor(sf::Color::Black);
    label->setString(name);
    label->setCharacterSize(Widget::kFontSize);
    label->setPosition(sf::Vector2f(-label->getLocalBounds().width/2,
                                    -label->getLocalBounds().height*.7));

    addDrawer("label", label);

    onMouseOut();
}

}
