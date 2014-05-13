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

#ifndef SFML_FLOW_CONTROLLER_H
#define SFML_FLOW_CONTROLLER_H

#include <SFML/Graphics.hpp>

#include "models.h"
#include "io.h"
#include "flow.h"

#include "basic_io.h"

namespace flow
{

class ControllerImpl;

class Controller
        : public sf::Drawable
        , public sf::Transformable
        , public Models
{
public:
    ~Controller();
    bool onEvent(const sf::Event& event, const sf::View& view);
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    static void setFont(const std::string& file);
    static void setWorkingDirectory(const std::string& path);
    int run();
    static Controller make()
    {
        return Controller(new BasicIO());
    }
    template <typename T>
    static Controller make()
    {
        return Controller(new T());
    }
private:
    Controller(const IO* io);
    ControllerImpl *ctrl_;
};

}

#endif // SFML_FLOW_CONTROLLER_H
