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

#include "node.h"
#include "rounded_rectangle.h"
#include "button.h"
#include "controller_impl.h"

namespace flow
{

Node::Node(
           const Model& model,
           const std::string& model_name,
                            boost::function<void(Node*)> remove_node,
                            boost::function<void(Anchor*)> start_anchor,
                            boost::function<void(Anchor*)> stop_anchor)
    : Widget(sf::Vector2f(100, 100))
    , model_name_(model_name)
    , model_(0)
{
    setMovable();
    // ok size depend of the numbers of inputs/outputs/parameters
    size_t max = model_name.size();
    for(Model::Data::Map::const_iterator it=model.inputs_.begin();
        it != model.inputs_.end(); ++it)
    {
        max = std::max(max, (*it).first.size());
    }
    for(Model::Data::Map::const_iterator it=model.outputs_.begin();
        it != model.outputs_.end(); ++it)
    {
        max = std::max(max, (*it).first.size());
    }
    for(Model::Data::Map::const_iterator it=model.parameters_.begin();
        it != model.parameters_.end(); ++it)
    {
        max = std::max(max, (*it).first.size());
    }
    max = std::max(max, model_name.size());

    int label_max_size = 10 * max; // magic number :(

    int size_y = model.inputs_.size() + model.outputs_.size() + model.parameters_.size();
    size_y = 20 * size_y + 30;
    size_ = sf::Vector2f(label_max_size, size_y);

    sf::RoundedRectangleShape* bg = new sf::RoundedRectangleShape(size_, float(M_PI*4.0f), 100u);
    bg->setFillColor(sf::Color(37, 37, 37));
    bg->setOutlineThickness(2);
    bg->setOutlineColor(sf::Color(68, 68, 68));
    bg->setOrigin(sf::Vector2f(size_.x/2, -size_.y/2));

    addDrawer("bg", bg);

    sf::Vector2f size;
    size.x = size_.x * .9;
    size.y = size_.y * .15;

    sf::Text* label = new sf::Text();
    label->setFont(getFont());
    label->setColor(sf::Color::White);
    label->setString(model_name);
    label->setCharacterSize(Widget::kFontSize);
    label->setPosition(sf::Vector2f(-label->getLocalBounds().width/2, -size_.y/2));

    addDrawer("label", label);

    double offset = -size_.y / 2 + 30;
    for(Model::Data::Map::const_iterator it=model.inputs_.begin();
        it != model.inputs_.end(); ++it)
    {
        Anchor::Ptr anchor = addChild<Anchor>(
                    boost::make_shared<Anchor>(
                        Flow::kInput,
                        (*it).first,
                        (*it).second,
                        start_anchor,
                        stop_anchor, this));
        anchor->setPosition(sf::Vector2f(-size_.x * 0.45-Anchor::kSize/2, offset));
        offset += 20;
    }

    for(Model::Data::Map::const_iterator it=model.parameters_.begin();
        it != model.parameters_.end(); ++it)
    {
        Anchor::Ptr anchor = addChild<Anchor>(
                    boost::make_shared<Anchor>(Flow::kParam, (*it).first, (*it).second, start_anchor, stop_anchor, this));
        anchor->setPosition(sf::Vector2f(-size_.x * 0.45-Anchor::kSize/2, offset));
        offset += 20;
    }

    for(Model::Data::Map::const_iterator it=model.outputs_.begin();
        it != model.outputs_.end(); ++it)
    {
        Anchor::Ptr anchor = addChild<Anchor>(boost::make_shared<Anchor>(Flow::kOutput, (*it).first, (*it).second, start_anchor, stop_anchor, this));
        anchor->setPosition(sf::Vector2f(size_.x * 0.45+Anchor::kSize/2, offset));
        offset += 20;
    }

    Button::Ptr b = addChild<Button>(
                boost::make_shared<Button>(sf::Vector2f(10, 14),
                                           "X",
                                           remove_node,
                                           this, Button::kCircle));
    b->setPosition(sf::Vector2f(size_.x*.5, -size_.y*.5));
}

Anchor::Ptr Node::getAnchor(Flow::Type type, const std::string& name)
{
    Widget::List list = getList<Anchor>();
    for(Widget::List::iterator it=list.begin();it!=list.end();++it)
    {
        Anchor::Ptr anchor = Widget::as<Anchor>(*it);
        if(anchor->getType() != type) continue;
        if(anchor->getName() == name) return anchor;
    }
    return Anchor::Ptr();
}

void Node::onMouseOver()
{
    render<sf::RectangleShape>("bg").setOutlineColor(sf::Color(172, 52, 149));
}

void Node::onMouseOut()
{
    render<sf::RectangleShape>("bg").setOutlineColor(sf::Color(68, 68, 68));
}

void Node::setParameter(const std::string& name, const std::string& value)
{
    Anchor::Ptr anchor = getAnchor(Flow::kParam, name);
    if(!anchor) return ;
    anchor->setValue(value);
}

}
