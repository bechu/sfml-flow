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

#ifndef SFML_FLOW_CURVE_H
#define SFML_FLOW_CURVE_H

#include <iostream>
#include <SFML/Graphics.hpp>

namespace flow
{

class Curve : public sf::Drawable, public sf::Transformable
{
public:
    Curve();
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    void add(const sf::Vector2f& point, unsigned int thickness=2, const sf::Color& color=sf::Color(255, 255, 0));
    void clear();
    void set(size_t index, const sf::Vector2f& point);
    void setColor(const sf::Color& color);
    size_t size()
    {
        return vertices_.getVertexCount();
    }
    virtual bool contains(const sf::Vector2f& coord);
private:
    sf::VertexArray vertices_;
    sf::Vector2f previous_;
    unsigned int pwidth_;
    sf::Color color_;
};

inline void Curve::set(size_t index, const sf::Vector2f& point)
{
    if(index < vertices_.getVertexCount())
    {
        vertices_[index].position = point;
    }
}

}

#endif // SFML_FLOW_CURVE_H
