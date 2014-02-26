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

#ifndef SFML_FLOW_INPUTDIALOG_H
#define SFML_FLOW_INPUTDIALOG_H

#include <vector>

#include "widget.h"
#include "input.h"
#include "toolgrid.h"

namespace flow
{

class InputDialog
        : public Widget
{
public:
    typedef boost::shared_ptr<InputDialog> Ptr;
    InputDialog(const sf::Vector2f& size);
    void setTitle(const std::string& text);
    void setValue(const std::string& value);
    void onOk(boost::function<void(const std::string&)> callback);
    void onCancel(boost::function<void()> callback);
    void setMode(bool open);
    void listFiles(
            boost::function<void(const std::string&)> load,
            boost::function<void(const std::string&)> save);
    void onOk();
    bool onKey(sf::Keyboard::Key key);
    bool onText(sf::Uint32 unicode);
private:
    Input::Ptr input_;
    Toolgrid::Ptr grid_;
    Button::Ptr ok_;
    Button::Ptr cancel_;
    bool open_;
    boost::function<void(const std::string&)> callback_;
};

}

#endif // SFML_FLOW_INPUTDIALOG_H
