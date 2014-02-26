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

#include "anchor.h"
#include "node.h"

namespace flow
{

Anchor::Anchor(Flow::Type type,
               const std::string& name,
               const Model::Data& data,
               boost::function<void(Anchor*)> start,
               boost::function<void(Anchor*)> stop,
               Node* parent)
    : Widget(sf::Vector2f(kSize, kSize))
    , name_(name)
    , data_(data)
    , type_(type)
    , start_(start)
    , stop_(stop)
    , parent_(parent)
{
    if(type == Flow::kParam)
    {
        sf::RectangleShape* bg = new sf::RectangleShape(sf::Vector2f(size_.x, size_.y));
        bg->setOrigin(sf::Vector2f(size_.x/2, size_.y/2));
        addDrawer("bg", bg);
    }
    else
    {
        sf::CircleShape* bg = new sf::CircleShape(size_.x/2, 200);
        bg->setOrigin(sf::Vector2f(size_.x/2, size_.x/2));
        addDrawer("bg", bg);
    }

    render<sf::Shape>("bg").setOutlineColor(sf::Color(68, 68, 68));
    render<sf::Shape>("bg").setOutlineThickness(1);
    onMouseOut();

    sf::Text* label = new sf::Text();
    label->setFont(getFont());
    label->setColor(sf::Color::White);
    label->setString(name);
    label->setCharacterSize(Widget::kFontSize);
    addDrawer("label", label);

    if(type == Flow::kOutput)
    {
        label->setPosition(sf::Vector2f(size_.x/2 - 10 - label->getLocalBounds().width, -label->getLocalBounds().height+size_.x/3));
    }
    else
    {
        label->setPosition(sf::Vector2f(size_.x/2 + 5, -label->getLocalBounds().height+size_.x/3));
    }

    tip_ = addChild<Tooltip>();
    tip_->setPosition(sf::Vector2f(30, 30));
    update_data();
}

void Anchor::update_data()
{
    if(type_ == Flow::kParam)
    {
        if(data_.is<int>())
            setValue<int>(data_.as<int>());
        if(data_.is<float>())
            setValue<float>(data_.as<float>());
        if(data_.is<double>())
            setValue<double>(data_.as<double>());
        if(data_.is<std::string>())
            setValue<std::string>(data_.as<std::string>());
    }
}

void Anchor::onMouseOver()
{
    render<sf::Shape>("bg").setFillColor(sf::Color(0, 0, 200));
    if(tip_ && type_ == Flow::kParam)
    {
        tip_->activate();
        tip_->show();
    }
}

void Anchor::onMouseOut()
{
    render<sf::Shape>("bg").setFillColor(sf::Color(237, 237, 47));
    if(tip_ && type_ == Flow::kParam)
    {
        tip_->deactivate();
        tip_->hide();
        if(type_ == Flow::kParam)
        {
            if(data_.is<int>())
                setValue<int>(getValue<int>());
            if(data_.is<float>())
                setValue<float>(getValue<float>());
            if(data_.is<double>())
                setValue<double>(getValue<double>());
            if(data_.is<std::string>())
                setValue<std::string>(getValue());
        }
    }
}

void Anchor::onMouseLeftPressed()
{
    start_(this);
}

bool Anchor::onMouseLeftReleased()
{
    stop_(this);
    return true;
}

void Anchor::setValue(const std::string& value)
{
    if(type_ == Flow::kParam)
    {
        if(data_.is<int>())
            data_.as<int>() = to<int>(value);
        if(data_.is<float>())
            data_.as<float>() = to<float>(value);
        if(data_.is<double>())
            data_.as<double>() = to<double>(value);
        if(data_.is<std::string>())
            data_.as<std::string>() = value;
        update_data();
    }
}

std::string Anchor::getName() const
{
    return name_;
}

Node* Anchor::getParent() const
{
    return parent_;
}

void Anchor::setType(Flow::Type type)
{
    type_ = type;
}

Flow::Type Anchor::getType() const
{
    return type_;
}

}
