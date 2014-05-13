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

#ifndef SFML_FLOW_UTIL_H
#define SFML_FLOW_UTIL_H

#include <cmath>

namespace flow
{

template <typename T>
inline static T length(const sf::Vector2<T>& a)
{
  return std::sqrt(a.x*a.x + a.y*a.y);
}

template <typename T>
inline sf::Vector2<T> normalize(const sf::Vector2<T>& vec)
{
  float length = std::sqrt(vec.x * vec.x + vec.y * vec.y);
  return length ? sf::Vector2<T>(vec.x / length, vec.y / length) : vec;
}

template <typename T>
inline T distance(const sf::Vector2<T>& from, const sf::Vector2<T>& to)
{
    return std::sqrt((from.x-to.x)*(from.x-to.x) + (from.y-to.y)*(from.y-to.y));
}

}

#endif // SFML_FLOW_UTIL_H
