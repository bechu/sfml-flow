SFML-FLOW [![Build Status](https://travis-ci.org/bechu/sfml-flow.png?branch=master)](https://travis-ci.org/bechu/sfml-flow)
=========

Flow editor, based on model definition.

![](images/screenshot.png?raw=true)

## Sample code

```c++
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

```
