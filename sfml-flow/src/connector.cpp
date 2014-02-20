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

#include "connector.h"
#include "node.h"

namespace flow
{

Connector::Connector(const Anchor* from, const Anchor* to,
                 boost::function<void(Connector*)> remove_connector)
    : from_(from)
    , to_(to)
    , remove_connector_(remove_connector)
{
    assert(from);
    assert(to);
    Curve* curve = new Curve();
    addDrawer("curve", curve);
    update();
}

void Connector::update()
{
    render<Curve>("curve").clear();
    render<Curve>("curve").add(from_->getPosition()+
                               from_->getParent()->getPosition(),
                               2, sf::Color(114, 114, 114));
    render<Curve>("curve").add(to_->getPosition()+
                               to_->getParent()->getPosition(),
                               2, sf::Color(114, 114, 114));
}

void Connector::onMouseOver()
{
    render<Curve>("curve").setColor(sf::Color(120, 40, 200));
}

void Connector::onMouseOut()
{
    render<Curve>("curve").setColor(sf::Color(114, 114, 114));
}

void Connector::onLeftDoubleClick()
{
    remove_connector_(this);
}

bool Connector::contains(const sf::Vector2f& coord, const sf::Vector2f& pose)
{
    return render<Curve>("curve").contains(coord - pose);
}

}
