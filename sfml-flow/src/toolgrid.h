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

#ifndef SFML_FLOW_TOOLGRID_H
#define SFML_FLOW_TOOLGRID_H

#include "widget.h"
#include "button.h"

namespace flow
{

class Toolgrid
        : public Widget
{
public:
    typedef boost::shared_ptr<Toolgrid> Ptr;
    Toolgrid(const sf::Vector2f& size);
    void add(const std::string& label,
               boost::function<void(const std::string&)> callback);
    void setColumn(u_int32_t column);
    void clear();
    void filter(const std::string& word);
private:
    const static int kXOffsetSize = 170;
    const static int kYOffsetSize = 40;
    const static int kButtonSize = 160;
    const static size_t kMaxCharacters = 20;
    float offset_x_;
    float offset_y_;
    u_int32_t column_;
};

inline void Toolgrid::setColumn(u_int32_t column)
{
    column_ = column;
}

}

#endif // SFML_FLOW_TOOLGRID_H
