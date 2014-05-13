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

#include "flow/controller.h"
#include "controller_impl.h"
#include "widget.h"

namespace flow
{

Controller::Controller(const IO *io)
{
    ctrl_ = new ControllerImpl(this, io);
}

Controller::~Controller()
{
    delete ctrl_;
    ctrl_ = 0;
}

void Controller::setFont(const std::string& file)
{
    Widget::setFont(file);
}

void Controller::setWorkingDirectory(const std::string& path)
{
    Widget::setWorkingDirectory(path);
}

bool Controller::onEvent(const sf::Event& event, const sf::View& view)
{
    return ctrl_->onEvent(event, view);
}

void Controller::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    ctrl_->draw(target, states);
}

int Controller::run()
{
    return ctrl_->run();
}

}
