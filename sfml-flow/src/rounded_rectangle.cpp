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

#include <cmath>
#include "rounded_rectangle.h"

namespace sf
{
////////////////////////////////////////////////////////////
RoundedRectangleShape::RoundedRectangleShape(const Vector2f& size, float radius, unsigned int cornerPointCount)
{
    mySize = size;
    myRadius = radius;
    myCornerPointCount = cornerPointCount;
    update();
}

////////////////////////////////////////////////////////////
void RoundedRectangleShape::setSize(const Vector2f& size)
{
    mySize = size;
    update();
}

////////////////////////////////////////////////////////////
const Vector2f& RoundedRectangleShape::getSize() const
{
    return mySize;
}

////////////////////////////////////////////////////////////
void RoundedRectangleShape::setCornersRadius(float radius)
{
    myRadius = radius;
    update();
}

////////////////////////////////////////////////////////////
const float RoundedRectangleShape::getCornersRadius() const
{
    return myRadius;
}

////////////////////////////////////////////////////////////
void RoundedRectangleShape::setCornerPointCount(unsigned int count)
{
    myCornerPointCount = count;
    update();
}

////////////////////////////////////////////////////////////
unsigned int RoundedRectangleShape::getPointCount() const
{
    return myCornerPointCount*4;
}

////////////////////////////////////////////////////////////
sf::Vector2f RoundedRectangleShape::getPoint(unsigned int index) const
{
    if(index >= myCornerPointCount*4)
        return sf::Vector2f(0,0);

    float deltaAngle = 90.0f/(myCornerPointCount-1);
    sf::Vector2f center;
    unsigned int centerIndex = index/myCornerPointCount;

    switch(centerIndex)
    {
        case 0: center.x = mySize.x - myRadius; center.y = myRadius; break;
        case 1: center.x = myRadius; center.y = myRadius; break;
        case 2: center.x = myRadius; center.y = mySize.y - myRadius; break;
        case 3: center.x = mySize.x - myRadius; center.y = mySize.y - myRadius; break;
    }

    return sf::Vector2f(myRadius*cos(deltaAngle*(index-centerIndex)*M_PI/180)+center.x,
                        myRadius*sin(deltaAngle*(index-centerIndex)*M_PI/180)-center.y);
}
}//namespace sf
