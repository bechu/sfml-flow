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

#ifndef SFML_FLOW_CONNECTOR_H
#define SFML_FLOW_CONNECTOR_H

#include <vector>

#include "anchor.h"
#include "widget.h"
#include "curve.h"

namespace flow
{

class Connector
        : public Widget
{
public:
    typedef boost::shared_ptr<Connector> Ptr;
    typedef std::vector<Connector::Ptr> List;
    Connector(const Anchor* from,
              const Anchor* to,
                 boost::function<void(Connector*)> remove_connector);
    void update();
    const Anchor* from();
    const Anchor* to();
    bool contains(const sf::Vector2f& coord,
                  const sf::Vector2f& pose);
    void onLeftDoubleClick();
    void onMouseOver();
    void onMouseOut();
private:
    const Anchor* from_;
    const Anchor* to_;
    boost::function<void(Connector*)> remove_connector_;
};

inline const Anchor* Connector::from()
{
    return from_;
}

inline const Anchor* Connector::to()
{
    return to_;
}

}

#endif // SFML_FLOW_CONNECTOR_H
