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

#include <exception>

#include "controller_impl.h"
#include "button.h"
#include "input.h"

namespace flow
{

ControllerImpl::ControllerImpl(Models* models, const IO* io)
    : models_(models)
    , initialized_(false)
    , run_(true)
    , io_(io)
{
}

ControllerImpl::~ControllerImpl()
{
}

namespace
{
sf::IntRect getViewport(const sf::View& view)
{
    float width  = static_cast<float>(view.getSize().x);
    float height = static_cast<float>(view.getSize().y);
    const sf::FloatRect& viewport = view.getViewport();

    return sf::IntRect(static_cast<int>(0.5f + width  * viewport.left),
                       static_cast<int>(0.5f + height * viewport.top),
                       static_cast<int>(width  * viewport.width),
                       static_cast<int>(height * viewport.height));
}
sf::Vector2f mapPixelToCoords(const sf::Vector2i& point, const sf::View& view)
{
    sf::Vector2f normalized;
    sf::IntRect viewport = getViewport(view);
    normalized.x = -1.f + 2.f * (point.x - viewport.left) / viewport.width;
    normalized.y =  1.f - 2.f * (point.y - viewport.top)  / viewport.height;
    return view.getInverseTransform().transformPoint(normalized);
}
}

void ControllerImpl::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    Widget::draw(target, states);
    states.transform *= getTransform();
}

bool ControllerImpl::onEvent(const sf::Event& event, const sf::View& view)
{
    if(initialized_ == false)
    {
        initialized_ = true;
        for(Models::iterator it=models_->begin();it!=models_->end();++it)
        {
            std::string name = (*it).first;
            grid_->add(name, boost::bind(&ControllerImpl::onAddNode, this, _1));
        }
    }
    if(event.type == sf::Event::MouseMoved)
    {
        sf::Vector2f coords = mapPixelToCoords(sf::Vector2i(event.mouseMove.x,
                                                            event.mouseMove.y), view);
        onMouseMove(coords);
        if(link_->editing())
            link_->update(coords);
    }
    else if(event.type == sf::Event::MouseButtonPressed)
    {
        sf::Vector2f coords = mapPixelToCoords(sf::Vector2i(event.mouseButton.x,
                                                            event.mouseButton.y), view);
        if(event.mouseButton.button == sf::Mouse::Right)
            onMouseRightPressed(coords);
        else
            onMouseLeftPressed(coords);
        if(clock_dbl_click_.getElapsedTime().asMilliseconds() <= 500)
        {
            if(coords == first_click_)
                onLeftDoubleClick(coords);
        }
        else
        {
            first_click_ = coords;
            clock_dbl_click_.restart();
        }
    }
    else if(event.type == sf::Event::MouseButtonReleased)
    {
        link_->edit(false);
        sf::Vector2f coords = mapPixelToCoords(sf::Vector2i(event.mouseButton.x,
                                                            event.mouseButton.y), view);
        if(state_ == kAddNode)
        {
            state_ = kDefault;
            flowImpl_->addNode(model_name_,
                               coords,
                               boost::bind(&ControllerImpl::onRemoveNode, this, _1),
                               boost::bind(&ControllerImpl::onStartAnchor, this, _1),
                               boost::bind(&ControllerImpl::onStopAnchor, this, _1));
            model_name_ = "";
        }
        if(event.mouseButton.button == sf::Mouse::Right)
            onMouseRightReleased(coords);
        else
            onMouseLeftReleased(coords);

    }
    else if(event.type == sf::Event::TextEntered)
    {
        onText(event.text.unicode);
    }
    else if (event.type == sf::Event::KeyPressed)
        onKey(event.key.code);
    return run_;
}

void ControllerImpl::onShowModels()
{
    grid_->show();
}

void ControllerImpl::onAddNode(const std::string& model)
{
    grid_->hide();
    state_ = kAddNode;
    model_name_ = model;
}

void ControllerImpl::onStartAnchor(Anchor* anchor)
{
    assert(anchor);
    link_->init(anchor->getPosition()+anchor->getParent()->getPosition());
    link_->update(anchor->getPosition()+anchor->getParent()->getPosition());
    link_->edit(true);
    start_anchor_ = anchor;
}

void ControllerImpl::onStopAnchor(Anchor* anchor)
{
    try
    {
        flowImpl_->connect(
                    start_anchor_,
                    anchor,
                    boost::bind(&ControllerImpl::onRemoveConnector, this, _1));
    }
    catch(const std::string& err)
    {
        onError(err);
    }
}

void ControllerImpl::onRemoveNode(Node *node)
{
    flowImpl_->removeNode(node);
}

void ControllerImpl::onRemoveConnector(Connector* connector)
{
    flowImpl_->removeConnector(connector);
}

void ControllerImpl::onClear()
{
    flowImpl_->clear();
}

void ControllerImpl::onClose()
{
    run_ = false;
}

int ControllerImpl::run()
{
    sf::RenderWindow window(sf::VideoMode(1200, 800), "flow", sf::Style::Default);
    window.setFramerateLimit(30);
    dialog_ = addChild<InputDialog>(
                boost::make_shared<InputDialog>(sf::Vector2f(window.getSize().x, window.getSize().y)));
    dialog_->setPosition(sf::Vector2f(window.getSize().x/2, window.getSize().y/2));
    dialog_->hide();

    msg_ = addChild<MsgBox>(
          boost::make_shared<MsgBox>(sf::Vector2f(window.getSize().x, window.getSize().y)));
    msg_->setPosition(sf::Vector2f(window.getSize().x/2, window.getSize().y/2));
    msg_->hide();
    link_ = addChild<EditConnector>();

    grid_ = addChild<Toolgrid>(boost::make_shared<Toolgrid>(sf::Vector2f(window.getSize().x, window.getSize().y)));
    grid_->hide();
    grid_->setColumn(6);
    //grid_->setPosition(sf::Vector2f(-window.getSize().x/2, 0));//window.getSize().y/2));
    grid_->setPosition(sf::Vector2f(0, 0));//window.getSize().y/2));

    Toolbar::Ptr toolbar = addChild<Toolbar>();
    toolbar->setPosition(sf::Vector2f(100, 30));
    toolbar->add("show models",
                 boost::bind(&ControllerImpl::onShowModels, this));
    toolbar->add("clear",
                 boost::bind(&ControllerImpl::onClear, this));
    toolbar->add("close",
                 boost::bind(&ControllerImpl::onClose, this));
    toolbar->add("open",
                 boost::bind(&ControllerImpl::onOpenFile, this));
    toolbar->add("save",
                 boost::bind(&ControllerImpl::onSaveFile, this));

    flowImpl_ = addChild<FlowImpl>(boost::make_shared<FlowImpl>(models_));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(onEvent(event, window.getView()) == false) window.close();
            if(event.type == sf::Event::Closed)
                window.close();
           // else if (event.type == sf::Event::KeyPressed)
           //     if (event.key.code == sf::Keyboard::Escape)
           //         window.close();
           // else if(event.type == sf::Event::Resized)
           //     {
           //         sf::View view = window.getView();
           //         view.setSize(event.size.width, event.size.height);
           //         window.setView(view);
           //     }
        }

        window.clear(sf::Color(56, 56, 56));

        window.draw(*this);

        window.display();
    }
    return 0;
}

void ControllerImpl::onOpenFile()
{
    dialog_->setMode(true);
    dialog_->listFiles(boost::bind(&ControllerImpl::onLoadFile, this, _1),
                       boost::bind(&ControllerImpl::onSaveFile, this, _1));
    dialog_->setTitle("Open a file");
    dialog_->onOk(boost::bind(&ControllerImpl::onLoadFile, this, _1));
    dialog_->onCancel(boost::bind(&ControllerImpl::onCloseDialog, this));
    dialog_->show();
}

void ControllerImpl::onSaveFile()
{
    dialog_->setMode(false);
    dialog_->listFiles(boost::bind(&ControllerImpl::onLoadFile, this, _1),
                       boost::bind(&ControllerImpl::onSaveFile, this, _1));
    dialog_->setTitle("Save a file");
    dialog_->onOk(boost::bind(&ControllerImpl::onSaveFile, this, _1));
    dialog_->onCancel(boost::bind(&ControllerImpl::onCloseDialog, this));
    dialog_->show();
}

void ControllerImpl::onLoadFile(const std::string& filename)
{
    dialog_->hide();
    dialog_->setValue(filename);
    try
    {
        flowImpl_->setFlow(io_->load(path_+filename),
                               boost::bind(&ControllerImpl::onRemoveConnector, this, _1),
                               boost::bind(&ControllerImpl::onRemoveNode, this, _1),
                               boost::bind(&ControllerImpl::onStartAnchor, this, _1),
                               boost::bind(&ControllerImpl::onStopAnchor, this, _1));
    }
    catch(const std::string& err)
    {
        onError(err);
    }
    catch(const std::exception& err)
    {
        onError(err.what());
    }
}

void ControllerImpl::onSaveFile(const std::string& filename)
{
    dialog_->hide();
    try
    {
        io_->save(path_+filename, flowImpl_->getFlow());
    }
    catch(const std::string& err)
    {
        onError(err);
    }
    catch(const std::exception& err)
    {
        onError(err.what());
    }
}

void ControllerImpl::onCloseDialog()
{
    dialog_->hide();
}

void ControllerImpl::onError(const std::string& error)
{
    msg_->setValue(error);
    msg_->show();
}

void ControllerImpl::onCloseMsgBox()
{
    msg_->hide();
}

}
