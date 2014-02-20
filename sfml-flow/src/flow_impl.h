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

#ifndef SFML_FLOW_FLOW_IMPL_H
#define SFML_FLOW_FLOW_IMPL_H

#include "node.h"
#include "anchor.h"
#include "connector.h"
#include "widget.h"
#include "flow/flow.h"
#include "flow/models.h"

namespace flow
{

class FlowImpl
        : public Widget
{
public:
    typedef boost::shared_ptr<FlowImpl> Ptr;
    FlowImpl(Models* models);
    void connect(Anchor *start,
                 Anchor *end,
                 boost::function<void(Connector*)> remove_connector);
    Node::Ptr addNode(const std::string model_name,
                      const sf::Vector2f &position,
                      boost::function<void(Node*)> remove_node,
                      boost::function<void(Anchor*)> start_anchor,
                      boost::function<void(Anchor*)> stop_anchor);
    void removeNode(Node *node);
    void removeConnector(Connector *connector);
    void setFlow(const Flow& flow,
                 boost::function<void(Connector*)> remove_connector,
                 boost::function<void(Node*)> remove_node,
                 boost::function<void(Anchor*)> start_anchor,
                 boost::function<void(Anchor*)> stop_anchor);
    Flow getFlow();
private:
    Models* models_;
};

}

#endif // SFML_FLOW_FLOW_IMPL_H
