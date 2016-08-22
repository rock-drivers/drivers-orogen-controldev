#ifndef CONTROLDEVTYPES_HPP
#define CONTROLDEVTYPES_HPP

#include <vector>
#include <string>
#include <base/Time.hpp>

namespace controldev
{

    /** A data structure for raw data values of input devices.
     *  Currently, only the Joystick and SliderBox inmput devices
     *  are supported.
     */
    struct RawCommand
    {
        /*
         * Device Type identifier like
         */
	std::string deviceIdentifier;

        /*
         * The Timesamp
         */
        base::Time time;

        /**
         * Axis values, scaled betwen 0 and 1.
         * */
        std::vector<double> axisValue;

        /*
         * Switch/Key Values
         * Three State switches are handles as two
         * Comment, this has to be handeld as an bool vecotr.
         * Sine bools are buggy in rock currently, workaround it with an uint8_t
         * vecotr. For more details see http://rock.opendfki.de/ticket/187
         */
        std::vector<uint8_t> buttonValue;
    };
}

#endif /* CONTROLDEVTYPES_HPP */
