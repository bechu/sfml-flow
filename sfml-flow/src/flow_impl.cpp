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

#include "flow_impl.h"
#include "button.h"

namespace flow
{

FlowImpl::FlowImpl(Models* models)
    : models_(models)
{
}

Node::Ptr FlowImpl::addNode(const std::string model_name,
                            const sf::Vector2f& position,
                            boost::function<void(Node*)> remove_node,
                            boost::function<void(Anchor*)> start_anchor,
                            boost::function<void(Anchor*)> stop_anchor)
{
    const Model* model = models_->getModel(model_name);
    Node::Ptr node = addChild<Node>(
                boost::make_shared<Node>(
                    *model,
                    model_name,
                    remove_node,
                    start_anchor,
                    stop_anchor));
    node->setPosition(position);
    return node;
}

void FlowImpl::connect(Anchor* start, Anchor *end,
                       boost::function<void(Connector*)> remove_connector)
{
    if(!start || !end) return ;
    if(start->getParent() == end->getParent())
        throw std::string("Impossible to connect node with itself !");

    if(start->getType() != Flow::kOutput)
        throw std::string("the first anchor must be an output !");

    if(end->getType() != Flow::kInput && end->getType() != Flow::kParam)
        throw std::string("the second anchor must be an input or parameter !");

    Widget::List list = getList<Connector>();
    for(Widget::List::iterator it=list.begin();it!=list.end();++it)
    {
        Connector::Ptr connector = Widget::as<Connector>(*it);
        if(connector->from() == start && connector->to() == end)
            throw std::string("this link already exist !");
    }
    Connector::Ptr connector = addChild<Connector>(boost::make_shared<Connector>(start, end, remove_connector));
    start->getParent()->addDepend(connector);
    end->getParent()->addDepend(connector);
    start->connected();
    end->connected();
}

void FlowImpl::removeNode(Node* node)
{
    Widget::List removed;
    Widget::List list = getList<Connector>();
    for(Widget::List::iterator it=list.begin();it!=list.end();++it)
    {
        Connector::Ptr connector = Widget::as<Connector>((*it));
        if(connector->from()->getParent() == node || connector->to()->getParent() == node)
            removed.push_back(connector);
    }
    for(Widget::List::iterator it=removed.begin();it!=removed.end();++it)
    {
        removeConnector(Widget::as<Connector>((*it)).get());
    }
    //removeChild(node);
    remove<Node>(node);
}

void FlowImpl::removeConnector(Connector* connector)
{
    Widget::List list = getList<Node>();
    for(Widget::List::iterator it=list.begin();it!=list.end();++it)
    {
        Node::Ptr node = Widget::as<Node>(*it);
        Widget::List list2 = node->getList<Anchor>();
        for(Widget::List::iterator it2=list2.begin();it2!=list2.end();++it2)
        {
            Anchor::Ptr anchor = Widget::as<Anchor>(*it2);
            if(anchor.get() == connector->from() || anchor.get() == connector->to())
            {
                anchor->disconnected();
            }
        }
    }
    connector->from()->getParent()->removeDepend(connector);
    connector->to()->getParent()->removeDepend(connector);
    //removeChild(connector);
    remove<Connector>(connector);
}

void FlowImpl::setFlow(const Flow& flow,
                       boost::function<void(Connector*)> remove_connector,
                       boost::function<void(Node*)> remove_node,
                       boost::function<void(Anchor*)> start_anchor,
                       boost::function<void(Anchor*)> stop_anchor)
{
    clear();
    std::vector<Node::Ptr> nodes;
    for(size_t i=0;i<flow.models_size();++i)
    {
        nodes.push_back(addNode(flow.getModelName(i),
                                sf::Vector2f(flow.getPosition(i).x, flow.getPosition(i).y),
                                remove_node, start_anchor, stop_anchor));
        std::map<std::string, std::string> params = flow.get_params(i);

        Node::Ptr node = nodes[i];
        for(std::map<std::string, std::string>::iterator it=params.begin();
            it != params.end(); ++it)
        {
            //std::cout<<"set flow "<<(*it).first<<" "<<(*it).second<<std::endl;
            node->setParameter((*it).first, (*it).second);
        }
    }
    for(size_t i=0;i<flow.links_size();++i)
    {
        Flow::Link link = flow.getLink(i);
        Node::Ptr from = nodes[link.from.index];
        Node::Ptr to = nodes[link.to.index];
        connect(from->getAnchor(link.from.type, link.from.name).get(),
                to->getAnchor(link.to.type, link.to.name).get(),
                remove_connector);
    }
}

Flow FlowImpl::getFlow()
{
    Flow flow;
    std::map<Node*, int> models;
    int i = 0;
    // list all models ...
    {
        Widget::List list = getList<Node>();
        for(Widget::List::iterator it=list.begin();it!=list.end();++it)
        {
            Node::Ptr node = Widget::as<Node>(*it);
            models[node.get()] = i++;
            flow.add(node->getModelName(),
                     node->getPosition().x,
                     node->getPosition().y);
            const Model* model = models_->getModel(node->getModelName());
            for(Model::Data::Map::const_iterator it=model->parameters_.begin();
                it != model->parameters_.end(); ++it)
            {
                flow.set_param(i-1, (*it).first, node->getAnchor(Flow::kParam, (*it).first)->getValue());
            }
        }
    }
    // list all connectors ...
    {
        Widget::List list = getList<Connector>();
        for(Widget::List::iterator it=list.begin();it!=list.end();++it)
        {
            Connector::Ptr connector = Widget::as<Connector>(*it);
            Flow::Link link;
            link.from.type = connector->from()->getType();
            link.from.name = connector->from()->getName();
            link.from.index = models[connector->from()->getParent()];
            link.to.type = connector->to()->getType();
            link.to.name = connector->to()->getName();
            link.to.index = models[connector->to()->getParent()];
            flow.add(link);
        }
    }
    return flow;
}

}
