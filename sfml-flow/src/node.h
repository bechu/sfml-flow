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

#ifndef SFML_FLOW_NODE_H
#define SFML_FLOW_NODE_H

#include "widget.h"
#include "flow/model.h"
#include "anchor.h"
#include "curve.h"

namespace flow
{

class Node
        : public Widget
{
public:
    typedef boost::shared_ptr<Node> Ptr;
    Node(const Model& model,
         const std::string& model_name,
         boost::function<void(Node*)> remove_node,
         boost::function<void(Anchor*)> start_anchor,
         boost::function<void(Anchor*)> stop_anchor);

    std::string getModelName() const;
    const Model* getModel() const;
    Anchor::Ptr getAnchor(Flow::Type type, const std::string& name);
    void onMouseOver();
    void onMouseOut();
    void setParameter(const std::string& name, const std::string& value);
private:
    std::string model_name_;
    const Model* model_;
};


inline const Model* Node::getModel() const
{
    return model_;
}

inline std::string Node::getModelName() const
{
    return model_name_;
}

}

#endif // SFML_FLOW_NODE_H
