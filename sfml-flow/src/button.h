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

#ifndef SFML_FLOW_BUTTON_H
#define SFML_FLOW_BUTTON_H

#include <string>
#include <map>
#include "widget.h"
#include "node.h"

namespace flow
{

class Button
        : public Widget
{
public:
    typedef enum
    {
        kRectangle,
        kCircle
    } Shape;
    typedef boost::shared_ptr<Button> Ptr;
    Button(const sf::Vector2f& size,
           const std::string& name,
           boost::function<void()> callback,
           Shape shape = kRectangle);

    Button(const sf::Vector2f& size,
           const std::string& name,
           Shape shape = kRectangle);

    Button(const sf::Vector2f& size,
           const std::string& name,
           boost::function<void(const std::string&)> callback,
           const std::string& value,
           Shape shape = kRectangle);

    Button(const sf::Vector2f& size,
           const std::string& name,
           boost::function<void(Node*)> callback,
           Node* node,
           Shape shape = kRectangle);

    void setCallback(boost::function<void()> callback);
    void setCallback(boost::function<void(const std::string &)> callback, const std::string &value);
    void call();
    std::string getLabel() const;
private:
    void setup(const std::string &name, Shape shape);
    void onMouseLeftPressed();
    bool onMouseLeftReleased();
    void onMouseOver();
    void onMouseOut();
    std::string value_;
    std::string name_;
    Node* node_;
    bool pushed_;
    boost::function<void()> callback_;
    boost::function<void(const std::string&)> callback2_;
    boost::function<void(Node*)> callback3_;
};

}

#endif // SFML_FLOW_BUTTON_H
