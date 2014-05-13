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


#include "include/flow/controller.h"

// fake class
struct Image {};
struct Position {};

int main()
{
    using namespace flow;

    // create a new controller with the basic input/output file management
    Controller ctrl = Controller::make();
    //  define the font
    Controller::setFont("font.ttf");
    // and where the controller load/save files
    Controller::setWorkingDirectory("./tmp/");

    // Create some models ...
    Model& open_image = ctrl.add("open_image");
    open_image.parameter<std::string>("filename") = "?";
    open_image.output<Image>("image");

    Model& bw = ctrl.add("black_and_white");
    bw.input<Image>("input");
    bw.output<Image>("done");

    Model& ff = ctrl.add("Find_Face");
    ff.parameter<std::string>("haar_cascade") = "default.xml";
    ff.output<Position>("position");
    ff.input<Image>("input");

    Model& dr = ctrl.add("Draw_Rectangle");
    dr.input<Position>("position");
    dr.parameter<std::string>("color");
    dr.parameter<int>("thickness") = 2;

    Model& di = ctrl.add("Draw_Image");
    di.parameter<Image>("image");
    //

    // that's it
    return ctrl.run();
}
