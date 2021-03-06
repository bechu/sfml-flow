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

#ifndef SFML_FLOW_ANCHOR_H
#define SFML_FLOW_ANCHOR_H

#include <vector>
#include <string>
#include <sstream>

#include "widget.h"

#include "flow/model.h"
#include "flow/flow.h"
#include "tooltip.h"

namespace flow
{

class Node;

class Anchor
        : public Widget
        , public boost::enable_shared_from_this<Anchor>
{
public:
    typedef boost::shared_ptr<Anchor> Ptr;
    typedef std::vector<Anchor::Ptr> List;

    const static int kSize = 10;

    Anchor(Flow::Type type,
           const std::string& name,
           const Model::Data& data,
           boost::function<void(Anchor*)> start,
           boost::function<void(Anchor*)> stop,
           Node * parent);

    void setType(Flow::Type type);
    Flow::Type getType() const;
    void onMouseLeftPressed();
    bool onMouseLeftReleased();
    Node* getParent() const;
    std::string getName() const;

    template <typename T>
    void setValue(T value);

    template <typename T>
    T getValue() const;

    template <typename T>
    T to(const std::string& str) const;

    std::string getValue() const;

    void setValue(const std::string& value);

    void connected();
    void disconnected();
private:
    void update_data();
    void onMouseOver();
    void onMouseOut();
    std::string name_;
    Model::Data data_;
    Flow::Type type_;
    boost::function<void(Anchor*)> start_;
    boost::function<void(Anchor*)> stop_;
    Node* parent_;

    Tooltip::Ptr tip_;
    sf::Color color_;
};

    template <typename T>
    void Anchor::setValue(T value)
    {
        if(!tip_) return ;
        std::stringstream ss;
        ss << value;
        tip_->setValue(ss.str());
    }

    template <typename T>
    T Anchor::getValue() const
    {
        if(!tip_) return T();
        std::stringstream ss(tip_->getValue());
        T u;
        ss >> u;
        return u;
    }

    template <typename T>
    T Anchor::to(const std::string& str) const
    {
        std::stringstream ss(str);
        T u;
        ss >> u;
        return u;
    }

}

#endif // SFML_FLOW_ANCHOR_H
