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

#ifndef SFML_FLOW_TOOLBAR_H
#define SFML_FLOW_TOOLBAR_H

#include "widget.h"

namespace flow
{

class Toolbar
        : public Widget
{
public:
    typedef boost::shared_ptr<Toolbar> Ptr;
    Toolbar();
    Widget::Ptr add(const std::string& label,
                    boost::function<void()> callback);
private:
    const static int kOffsetSize = 150;
    const static int kButtonSize = 100;
    float offset_;
};

}

#endif // SFML_FLOW_TOOLBAR_H
