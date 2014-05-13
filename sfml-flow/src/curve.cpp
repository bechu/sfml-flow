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

#include "curve.h"
#include "util.h"
#include <iostream>

namespace flow
{

Curve::Curve()
{
    previous_ = sf::Vector2f(-1, -1);
    vertices_.setPrimitiveType(sf::Points);
    vertices_.setPrimitiveType(sf::TrianglesStrip);
}

void Curve::clear()
{
    vertices_.clear();
    previous_ = sf::Vector2f(-1, -1);
}

void Curve::setColor(const sf::Color& color)
{
    for(size_t i=0;i<vertices_.getVertexCount();++i)
    {
        vertices_[i].color = color;
    }
}

void Curve::add(const sf::Vector2f& point, unsigned int thickness, const sf::Color& color)
{
    if(previous_.x != -1 && previous_.y != -1)
    {
        sf::Vector2f p1 = previous_;
        sf::Vector2f p2 = point;
        sf::Vector2f  v = p2 - p1;

        v /= length(v);  // make it a unit vector
        sf::Vector2f  vp(-v.y, v.x);  // compute the vector perpendicular to v

        sf::Vector2f temp;
        temp.x = p1.x + pwidth_/2 * vp.x;
        temp.y = p1.y + pwidth_/2 * vp.y;
        vertices_.append(sf::Vertex(temp, color_));

        temp.x = p1.x - pwidth_/2 * vp.x;
        temp.y = p1.y - pwidth_/2 * vp.y;
        vertices_.append(sf::Vertex(temp, color_));

        temp.x = p2.x + thickness/2 * vp.x;
        temp.y = p2.y + thickness/2 * vp.y;
        vertices_.append(sf::Vertex(temp, color));

        temp.x = p2.x - thickness/2 * vp.x;
        temp.y = p2.y - thickness/2 * vp.y;
        vertices_.append(sf::Vertex(temp, color));
    }
    previous_ = point;
    pwidth_ = thickness;
    color_ = color;
}

void Curve::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    target.draw(vertices_, states);
}

namespace
{
int pnpoly(const std::vector<sf::Vector2f>& points, const sf::Vector2f& test)
{
    unsigned int i, j;
    int c = 0;

    for (i = 0, j = points.size()-1; i < points.size(); j = i++)
    {
        if ( ((points[i].y > test.y) != (points[j].y > test.y)) &&
             (test.x < (points[j].x-points[i].x)
              * (test.y-points[i].y) / (points[j].y-points[i].y) + points[i].x) )
        {
            c = !c;
        }
    }
    return c;
}
}

bool Curve::contains(const sf::Vector2f& coord)
{
    std::vector<sf::Vector2f> points;
    for(unsigned int i=0;i<=vertices_.getVertexCount()-4;i+=2)
    {
        points.clear();
        points.push_back(vertices_[i+0].position);
        points.push_back(vertices_[i+1].position);
        points.push_back(vertices_[i+3].position);
        points.push_back(vertices_[i+2].position);
        if(pnpoly(points, coord))
        {
            return true;
        }
    }
    return false;
}

}
