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

#include <boost/filesystem.hpp>

#include "input_dialog.h"
#include "button.h"

namespace flow
{

InputDialog::InputDialog(const sf::Vector2f &size)
    : Widget(size)
    , open_(true)
{
    sf::RectangleShape* bg = new sf::RectangleShape(size);
    bg->setFillColor(sf::Color(10, 10, 10, 200));
    bg->setOrigin(sf::Vector2f(size.x/2, size.y/2));
    addDrawer("bg", bg);

    sf::Text* label = new sf::Text();
    label->setFont(getFont());
    label->setColor(sf::Color::White);
    label->setCharacterSize(kFontSize);
    label->setPosition(sf::Vector2f(-label->getLocalBounds().width,
                                    -label->getLocalBounds().height*.7-300));
    addDrawer("label", label);

    input_ = addChild<Input>();
    input_->setPosition(sf::Vector2f(0, 300));

    ok_ = addChild<Button>(boost::make_shared<Button>(sf::Vector2f(100, 30), "OK"));
    ok_->setPosition(sf::Vector2f(-100, 350));
    ok_->setCallback(boost::bind(&InputDialog::onOk, this));

    cancel_ = addChild<Button>(boost::make_shared<Button>(sf::Vector2f(100, 30), "Cancel"));
    cancel_->setPosition(sf::Vector2f(100, 350));

    grid_ = addChild<Toolgrid>(boost::make_shared<Toolgrid>(size));
    grid_->setPosition(sf::Vector2f(-size.x/2, -size.y/2));
    grid_->setColumn(7);
}

void InputDialog::onOk()
{
    std::string input = input_->getText();
    if(input.size() > 0) callback_(input);
    else
        hide();
}

void InputDialog::onOk(
        boost::function<void(const std::string&)> callback)
{
    callback_ = callback;
}

void InputDialog::onCancel(
        boost::function<void()> callback)
{
    cancel_->setCallback(callback);
}

void InputDialog::setValue(const std::string& value)
{
    input_->setText(value);
}

void InputDialog::setTitle(const std::string& text)
{
    render<sf::Text>("label").setString(text);
}

void InputDialog::listFiles(boost::function<void(const std::string&)> load,
                            boost::function<void(const std::string&)> save)
{
    grid_->clear();
    boost::filesystem::path p(path_);

    boost::filesystem::directory_iterator end_iter;
    boost::filesystem::directory_iterator it(p);
    while(it != end_iter)
    {
        boost::filesystem::path t = *it;
        std::string f = t.string();
        std::size_t pos = f.rfind("/");
        if(pos == std::string::npos) continue ;
        if(f.size() < pos+1) continue ;
        std::string u = f.substr(pos+1, f.size());
        if(open_)
            grid_->add(u, load);
        else
            grid_->add(u, save);
        ++it;
    }
}

void InputDialog::setMode(bool open)
{
    open_ = open;
}

bool InputDialog::onKey(sf::Keyboard::Key key)
{
    if(key == sf::Keyboard::Return)
    {
        onOk();
        return true;
    }
    else if(key == sf::Keyboard::Escape)
    {
        hide();
        return true;
    }
    return Widget::onKey(key);
}

bool InputDialog::onText(sf::Uint32 unicode)
{
    bool ret = Widget::onText(unicode);
    grid_->filter(input_->getText());
    return ret;
}

}
