#ifndef CONTROLDEVTYPES_HPP
#define CONTROLDEVTYPES_HPP

#include <string>

namespace controldev
{

    struct AxisPort
    {
        //name of the to be created port
        std::string portName;
        //axis number of the port that should be written out
        int axisNr;
        //scale factor that is applied to the output value
        double scaleFactor;
        //offset that will be applied to the output value
        double offset;
    };
}

#endif /* CONTROLDEVTYPES_HPP */
