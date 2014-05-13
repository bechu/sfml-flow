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

#ifndef SFML_FLOW_CONTROLLER_IMPL_H
#define SFML_FLOW_CONTROLLER_IMPL_H

#include <SFML/Window.hpp>
#include <map>

#include "flow/model.h"
#include "flow/io.h"

#include "widget.h"
#include "flow/models.h"
#include "toolbar.h"
#include "toolgrid.h"
#include "flow_impl.h"
#include "input_dialog.h"
#include "msgbox.h"

namespace flow
{

class ControllerImpl
        : public Widget
{
public:
    ControllerImpl(Models* models, const IO* io);
    ~ControllerImpl();
    bool onEvent(const sf::Event& event, const sf::View& view);

    void onShowModels();

    void onClear();
    void onClose();
    void onAddNode(const std::string& model);
    void onStartAnchor(Anchor* anchor);
    void onStopAnchor(Anchor* anchor);
    void onRemoveNode(Node* node);
    void onRemoveConnector(Connector *connector);
    void onOpenFile();
    void onSaveFile();
    void onLoadFile(const std::string& filename);
    void onSaveFile(const std::string& filename);
    void onCloseDialog();
    void onError(const std::string& error);
    virtual void onCloseMsgBox();

    int run();
private:
    typedef enum
    {
        kDefault,
        kAddNode
    } State;
    State state_;
    Models* models_;
    FlowImpl::Ptr flowImpl_;
    Toolgrid::Ptr grid_;
    InputDialog::Ptr dialog_;
    MsgBox::Ptr msg_;
    bool initialized_;
    sf::Texture process_;
    std::string model_name_;
    sf::Clock clock_dbl_click_;
    sf::Vector2f first_click_;
    struct EditConnector
            : public Widget
    {
        typedef boost::shared_ptr<EditConnector> Ptr;
        EditConnector()
        {
            addDrawer("curve", new Curve());
            edit(false);
        }
        void init(const sf::Vector2f& from)
        {
            render<Curve>("curve").clear();
            render<Curve>("curve").add(from, 8, sf::Color::Red);
            render<Curve>("curve").add(sf::Vector2f(100, 100), 8, sf::Color::Red);
            from_ = from;
        }
        void update(const sf::Vector2f& to)
        {
            render<Curve>("curve").clear();
            render<Curve>("curve").add(from_, 8, sf::Color::Red);
            render<Curve>("curve").add(to, 8, sf::Color::Red);
        }
        bool editing() const
        {
            return editing_;
        }
        void edit(bool ed)
        {
            if(ed) show();
            else hide();
            editing_ = ed;
        }
        private:
        bool editing_;
        sf::Vector2f from_;
    };
    EditConnector::Ptr link_;
    Anchor* start_anchor_;
    bool run_;
    const IO* io_;
};

}

#endif // SFML_FLOW_CONTROLLER_IMPL_H
