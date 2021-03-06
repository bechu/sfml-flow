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

#include "widget.h"

namespace flow
{

sf::Font  Widget::font_;
std::string  Widget::path_;
int  Widget::sorted_counter_ = 0;

Widget::Widget(const sf::Vector2f& size)
    : size_(size)
    , over_(false)
    , moving_(false)
    , movable_(false)
    , visible_(true)
    , enable_(true)
{
    sorted_.resize(100);
}

Widget::~Widget()
{
}

sf::Font&  Widget::getFont()
{
    return font_;
}

void Widget::addDrawer(const std::string& name, sf::Drawable *d)
{
    draws_[name] = d;
}

void  Widget::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    if(visible_ == false) return ;
    states.transform *= getTransform();
    for(std::map<std::string, sf::Drawable*>::const_iterator it=draws_.begin();it!=draws_.end();++it)
    {
        const sf::Drawable* drawer = (*it).second;
        target.draw(*drawer, states);
    }
    for(std::vector<Widget::List>::const_reverse_iterator sit=sorted_.rbegin();
        sit != sorted_.rend(); ++sit)
    {
        for(Widget::List::const_iterator it=(*sit).begin();it!=(*sit).end();++it)
        {
            const  Widget::Ptr widget = *it;
            if(std::find(hide_.begin(), hide_.end(), widget) == hide_.end())
                target.draw(*widget, states);
        }
    }
}

bool  Widget::onMouseMove(const sf::Vector2f& coord, const sf::Vector2f& position)
{
    if(isVisible() == false) return false;
    for(std::vector<Widget::List>::iterator sit=sorted_.begin();
        sit != sorted_.end(); ++sit)
    {
        for(Widget::List::iterator it=(*sit).begin();it!=(*sit).end();++it)
        {
            Widget::Ptr widget = *it;
            if(widget->onMouseMove(coord, position+getPosition())) return true;
        }
    }
    if(moving_)
    {
        move(coord-start_pos_);
        update();
        update_depends();
        start_pos_ = coord;
        return true;
    }
    if(contains(coord, position+getPosition()))
    {
        if(over_ == false)
        {
            onMouseOver();
        }
        over_ = true;
        return true;
    }
    if(over_ == true)
    {
        over_ = false;
        onMouseOut();
    }
    return false;
}

void  Widget::removeDepend( Widget* widget)
{
    for( Widget::List::iterator it=depends_.begin();it!=depends_.end();++it)
    {
        if((*it).get() == widget)
        {
            depends_.erase(it);
            return ;
        }
    }
}

bool  Widget::onMouseLeftPressed(const sf::Vector2f& coord, const sf::Vector2f &position)
{
    if(isVisible() == false) return false;

    for(std::vector<Widget::List>::iterator sit=sorted_.begin();
        sit != sorted_.end(); ++sit)
        for( Widget::List::iterator it=(*sit).begin();it!=(*sit).end();++it)
            if((*it)->onMouseLeftPressed(coord, position+getPosition())) return true;
    if(contains(coord, getPosition()+position))
    {
        if(movable_)
        {
            moving_ = true;
            start_pos_ = coord;
        }
        onMouseLeftPressed();
        return true;
    }
    return false;
}

void  Widget::onMouseRightPressed(const sf::Vector2f& coord, const sf::Vector2f& position)
{
    if(isVisible() == false) return ;
    for(std::vector<Widget::List>::iterator sit=sorted_.begin();
        sit != sorted_.end(); ++sit)
        for( Widget::List::iterator it=(*sit).begin();it!=(*sit).end();++it)
            (*it)->onMouseLeftPressed(coord, position+getPosition());
    if(contains(coord, getPosition()+position)) onMouseRightPressed();
}

bool  Widget::onMouseLeftReleased(const sf::Vector2f& coord, const sf::Vector2f& position)
{
    if(isVisible() == false) return false;
    moving_ = false;
    for(std::vector<Widget::List>::iterator sit=sorted_.begin();
        sit != sorted_.end(); ++sit)
        for( Widget::List::iterator it=(*sit).begin();it!=(*sit).end();++it)
        {
            if((*it)->onMouseLeftReleased(coord, position+getPosition()) == true)
                return true;
        }
    if(isVisible() == false) return false;
    if(contains(coord, getPosition()+position))
    {
        onMouseLeftReleased();
        return true;
    }
    return false;
}

void  Widget::onMouseRightReleased(const sf::Vector2f& coord, const sf::Vector2f& position)
{
    if(isVisible() == false) return ;
    for(std::vector<Widget::List>::iterator sit=sorted_.begin();
        sit != sorted_.end(); ++sit)
        for( Widget::List::iterator it=(*sit).begin();it!=(*sit).end();++it)
            (*it)->onMouseRightReleased(coord, position+getPosition());
    if(contains(coord, getPosition()+position)) onMouseRightReleased();
}

bool  Widget::onLeftDoubleClick(const sf::Vector2f& coord)
{
    if(isVisible() == false) return false;
    for(std::vector<Widget::List>::iterator sit=sorted_.begin();
        sit != sorted_.end(); ++sit)
        for( Widget::List::iterator it=(*sit).begin();it!=(*sit).end();++it)
        {
            Widget::Ptr widget = *it;
            if(widget->onLeftDoubleClick(coord)) return true;
        }
    if(contains(coord, getPosition()))
    {
        onLeftDoubleClick();
        return true;
    }
    return false;
}

void  Widget::update_depends()
{
    if(isVisible() == false) return ;
    for( Widget::List::iterator it=depends_.begin();it!=depends_.end();++it)
    {
        Widget::Ptr widget = *it;
        widget->update();
        widget->update_depends();
    }
}

void  Widget::setFont(const std::string& file)
{
    font_.loadFromFile(file);
}

void  Widget::setWorkingDirectory(const std::string& path)
{
    path_ = path;
}


bool Widget::onText(sf::Uint32 unicode)
{
    if(isVisible() == false) return false;
    for(std::vector<Widget::List>::iterator sit=sorted_.begin();
        sit != sorted_.end(); ++sit)
    {
        for(Widget::List::iterator it=(*sit).begin();it!=(*sit).end();++it)
        {
            Widget::Ptr widget = *it;
            if(widget->onText(unicode) == true)
                return true;
        }
    }
    return false;
}

bool Widget::onKey(sf::Keyboard::Key key)
{
    if(isVisible() == false) return false;

    for(std::vector<Widget::List>::iterator sit=sorted_.begin();
        sit != sorted_.end(); ++sit)
    {
        for(Widget::List::iterator it=(*sit).begin();it!=(*sit).end();++it)
        {
            Widget::Ptr widget = *it;
            if(widget->isVisible() == true)
                if(widget->onKey(key) == true)
                    return true;
        }
    }

    return false;
}

void  Widget::clear()
{
    sorted_.clear();
    sorted_.resize(100);
}

bool Widget::contains(const sf::Vector2f& coord, const sf::Vector2f& pose)
{
    sf::FloatRect boudingBox;
    boudingBox.left = pose.x - size_.x / 2;
    boudingBox.top = pose.y - size_.y / 2;
    boudingBox.width = size_.x;
    boudingBox.height = size_.y;
    return boudingBox.contains(coord);
}

int Widget::getNextSlot()
{
    return sorted_counter_++;
}

void Widget::setMovable()
{
    movable_ = true;
}

void Widget::hide()
{
    visible_ = false;
}

void Widget::show()
{
    visible_ = true;
}

bool Widget::isVisible() const
{
    return visible_;
}

const sf::Vector2f& Widget::getSize() const
{
    return size_;
}

void Widget::addDepend(Widget::Ptr widget)
{
    depends_.push_back(widget);
}

}
