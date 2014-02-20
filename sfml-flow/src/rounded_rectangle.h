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

#ifndef ROUNDEDRECTANGLESHAPE_HPP
#define ROUNDEDRECTANGLESHAPE_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Shape.hpp>

namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Specialized shape representing a rectangle
/// with rounded corners
////////////////////////////////////////////////////////////
class RoundedRectangleShape : public sf::Shape
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// \param size Size of the rectangle
    /// \param radius Radius for each rounded corner
    /// \param pointCount Number of points of each corner
    ///
    ////////////////////////////////////////////////////////////
    explicit RoundedRectangleShape(const Vector2f& size = Vector2f(0, 0), float radius = 0, unsigned int cornerPointCount = 0);

    ////////////////////////////////////////////////////////////
    /// \brief Set the size of the rounded rectangle
    ///
    /// \param size New size of the rounded rectangle
    ///
    /// \see GetSize
    ///
    ////////////////////////////////////////////////////////////
    void setSize(const Vector2f& size);

    ////////////////////////////////////////////////////////////
    /// \brief Get the size of the rounded rectangle
    ///
    /// \return Size of the rounded rectangle
    ///
    /// \see SetSize
    ///
    ////////////////////////////////////////////////////////////
    const Vector2f& getSize() const;

    ////////////////////////////////////////////////////////////
    /// \brief Set the radius of the rounded corners
    ///
    /// \param Radius of the rounded corners
    ///
    /// \see GetRadius
    ///
    ////////////////////////////////////////////////////////////
    void setCornersRadius(float radius);

    ////////////////////////////////////////////////////////////
    /// \brief Get the radius of the rounded corners
    ///
    /// \return Radius of the rounded corners
    ///
    /// \see SetRadius
    ///
    ////////////////////////////////////////////////////////////
    const float getCornersRadius() const;

    ////////////////////////////////////////////////////////////
    /// \brief Set the number of points of each corner
    ///
    /// \param count New number of points of the rounded rectangle
    ///
    /// \see GetPointCount
    ///
    ////////////////////////////////////////////////////////////
    void setCornerPointCount(unsigned int count);

    ////////////////////////////////////////////////////////////
    /// \brief Get the number of points defining the rounded rectangle
    ///
    /// \return Number of points of the rounded rectangle
    ///
    ////////////////////////////////////////////////////////////
    virtual unsigned int getPointCount() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get a point of the rounded rectangle
    ///
    /// The result is undefined if \a index is out of the valid range.
    ///
    /// \param index Index of the point to get, in range [0 .. GetPointCount() - 1]
    ///
    /// \return Index-th point of the shape
    ///
    ////////////////////////////////////////////////////////////
    virtual sf::Vector2f getPoint(unsigned int index) const;

private:
    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    Vector2f mySize;
    float myRadius;
    unsigned int myCornerPointCount;
};
}
#endif // ROUNDEDRECTANGLESHAPE_HPP
