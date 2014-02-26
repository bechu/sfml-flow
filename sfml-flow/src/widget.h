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

#ifndef SFML_FLOW_WIDGET_H
#define SFML_FLOW_WIDGET_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <set>
#include <algorithm>
#include <cassert>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

#include "util.h"

namespace flow
{

////////////////////////////////////////////////////////////
/// \brief widget implementation
////////////////////////////////////////////////////////////
class Widget
        : public sf::Drawable
        , public sf::Transformable
{
public:
    typedef boost::shared_ptr<Widget> Ptr;
    typedef std::vector<Widget::Ptr> List;

    void hide();

    virtual void show();

    bool isVisible() const;

    void addDepend(Widget::Ptr widget);

    void removeDepend(Widget* widget);

    virtual void clear();

    static void setFont(const std::string& file);

    static void setWorkingDirectory(const std::string& path);

protected:
    const static int kFontSize = 15;
    Widget(const sf::Vector2f& size=sf::Vector2f());
    virtual ~Widget();

    // widget children
    template <typename T>
    boost::shared_ptr<T> addChild();

    template <typename T>
    boost::shared_ptr<T> addChild(Widget::Ptr widget);

    template <typename T>
    List getList();

    template <typename T>
    inline static boost::shared_ptr<T> as(Widget::Ptr w)
    {
        return boost::static_pointer_cast<T>(w);
    }

    template <typename T>
    void remove(T* inst);

    void removeChild(Widget *widget);

    // basic 2d shape
    template <typename T>
    T& render(const std::string& name);
    void addDrawer(const std::string &name, sf::Drawable *d);

    // font
    sf::Font& getFont();

    // SFML draw primitive
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

    // features
    sf::Vector2f getSize() const;
    void setFeatures(unsigned char features);
    void setMovable();

protected:
    virtual bool onText(sf::Uint32 unicode);
    virtual bool onKey(sf::Keyboard::Key key);

    virtual void onMouseOver() {}
    virtual void onMouseOut() {}

    virtual bool onMouseLeftPressed(const sf::Vector2f& coord,
                                    const sf::Vector2f& position=sf::Vector2f());
    virtual void onMouseRightPressed(const sf::Vector2f& coord,
                                     const sf::Vector2f& position=sf::Vector2f());
    virtual bool onMouseLeftReleased(const sf::Vector2f& coord,
                                     const sf::Vector2f& position=sf::Vector2f());
    virtual void onMouseRightReleased(const sf::Vector2f& coord,
                                      const sf::Vector2f& position=sf::Vector2f());
    virtual void onMouseLeftPressed() {}
    virtual void onMouseRightPressed() {}
    virtual bool onMouseLeftReleased() {return false;}
    virtual void onMouseRightReleased() {}

    bool onMouseMove(const sf::Vector2f& coord, const sf::Vector2f& position=sf::Vector2f());

    bool onLeftDoubleClick(const sf::Vector2f& coord);

    virtual void onMouseDown() {}
    virtual void onMouseUp() {}
    virtual void onLeftDoubleClick() {}

    static std::string path_;
    sf::Vector2f size_;
protected:
    Widget::List children_;
private:
    virtual bool contains(const sf::Vector2f& coord, const sf::Vector2f &pose);
    virtual void update() {}
    void update_depends();

    Widget::Ptr addChild(Widget::Ptr widget);

    template <typename T>
    int slot();

    int getNextSlot();

    static sf::Font font_;
    sf::Vector2f start_pos_;
    bool moving_;
    Widget::List depends_;
    std::set<Widget::Ptr> hide_;
    std::vector<Widget::List> sorted_;
    bool enable_;
    bool visible_;
    bool movable_;
    static int sorted_counter_;
    bool over_;
    std::map<std::string, sf::Drawable*> draws_;
};

template <typename T>
boost::shared_ptr<T> Widget::addChild(Widget::Ptr widget)
{
    int s = slot<T>();
    sorted_[s].push_back(widget);
    addChild(widget);
    return boost::static_pointer_cast<T>(widget);
}

template <typename T>
void Widget::remove(T* inst)
{
    int s = slot<T>();
    if(s < sorted_.size())
    {
        for(Widget::List::iterator it=sorted_[s].begin();
            it!= sorted_[s].end();++it)
        {
            if((*it).get() == inst)
            {
                sorted_[s].erase(it);
                return ;
            }
        }
    }
}

template <typename T>
boost::shared_ptr<T> Widget::addChild()
{
    int s = slot<T>();
    boost::shared_ptr<T> t = boost::make_shared<T>();
    sorted_[s].push_back(t);
    children_.push_back(sorted_[s].back());
    //sort();
    return t;
}

template <typename T>
inline Widget::List Widget::getList()
{
    int s = slot<T>();
    if(s < sorted_.size())
        return sorted_[s];
    return Widget::List();
}

template <typename T>
inline int Widget::slot()
{
    static const int slot = getNextSlot();
    return slot;
}

template <typename T>
T& Widget::render(const std::string& name)
{
    assert(draws_.find(name) != draws_.end());
    return *(reinterpret_cast<T*>(draws_[name]));
}

}

#endif // SFML_FLOW_WIDGET_H
