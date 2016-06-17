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

        /* Index 1: num-of input axis, //index 2: dimensions of this axis
         * If you have an gamepand which has 2 2Dknops you have an [2][2] 
         * size'd array for an 3D Mouse you could have [1][6]
         */
        std::vector<std::vector<double> > axisValue;

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
