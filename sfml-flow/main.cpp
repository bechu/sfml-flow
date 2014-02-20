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

#include "include/flow/controller.h"

int main()
{
    using namespace flow;

    // I create a new controller with a class
    // to serialize or unserialize the flow
    Controller ctrl = Controller::make<BasicIO>();

    // define the font used for the UI
    Controller::setFont("font.ttf");
    // define where the editor load/save files
    Controller::setWorkingDirectory("tmp/");

    // create a first model : random
    Model& random = ctrl.add("random");
    random.parameter<float>("min") = 0;
    random.parameter<float>("max") = 3;
    random.output<float>("out");

    // create a second model : print
    Model& print = ctrl.add("print");
    print.input<float>("in");

    Model& addition = ctrl.add("Addition");
    addition.input<float>("A");
    addition.input<float>("B");
    addition.output<float>("Result");

    // launch interface & that's it !
    return ctrl.run();
}



























class XmlOutput
        : public flow::IO
{
public:
    void save(const std::string& filename,
              const flow::Flow& flow) const
    {
    }
    flow::Flow load(const std::string& filename) const
    {
        flow::Flow flow;
        return flow;
    }
};
