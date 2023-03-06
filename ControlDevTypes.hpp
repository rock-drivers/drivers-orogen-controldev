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

    enum ControlMode
    {
        ControlOff = 0,
        Control,
        KeepAlive,
        Timeout
    };

    struct ButtonMapping
    {
        int control_off;
        int control;
        int keep_alive;
        int timeout;

        ButtonMapping() : control_off(-1), control(-1), keep_alive(-1), timeout(-1) {}
    };
}

#endif /* CONTROLDEVTYPES_HPP */
