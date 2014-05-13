SFML-FLOW
=========

[![Build Status](https://travis-ci.org/bechu/sfml-flow.png?branch=master)](https://travis-ci.org/bechu/sfml-flow)

Flow editor, based on model definition in c++.

Required :
 - SFML 2.1
 - Boost (system, filesystem, bind, functors)

## What it's look like :

(Click on the screenshot to see the video !)


[![Flow Demo](http://img.youtube.com/vi/-0VJ6-2Nrpw/0.jpg)](http://www.youtube.com/watch?v=-0VJ6-2Nrpw)

## Corresponding sample code

```c++
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

```

## Specialize the input/output

Of course is possible to define how to export and load data :

```c++
class XmlOutput
        : public flow::IO
{
public:
    void save(const std::string& filename,
              const flow::Flow& flow) const
    {
      // save the flow structure to xml file ...
    }
    flow::Flow load(const std::string& filename) const
    {
        flow::Flow flow;
        // load xml data and fill the flow structure ...
        return flow;
    }
};
```

In this case just create a controller with this new class :

```c++
Controller ctrl = Controller::make<XmlOutput>();
```

