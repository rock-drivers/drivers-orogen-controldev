#ifndef CONTROLDEVTYPES_HPP
#define CONTROLDEVTYPES_HPP

#include <vector>
#include <string>
#include <base/Time.hpp>
#include <base/NamedVector.hpp>

namespace controldev
{

    /** \deprecated See the documentation of Command
    * The control mode requested for a controller output
    *
    */
    enum DRIVE_MODE
    {
        DM_PWM = 0, //! direct duty control
        DM_SPEED = 1, //! speed control
        DM_POSITION = 2, //! position control
        DM_UNINITIALIZED = 3
    };


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
         * The time stamp
         */
        base::Time time;

        /* Named vector for the axis */
        base::NamedVector <double> axes;

        /*
         * Named vector Switch/Key Values
         * Three State switches are handles as two
         * Comment, this has to be handle as an boolean vector.
         * Sine boolean are buggy in rock currently, workaround it with an uint8_t
         * vector. For more details see http://rock.opendfki.de/ticket/187
         */
        base::NamedVector <int> buttons;
    };

/** Data structure to send separate commands for each motor */
    struct FourWheelCommand
    {
        DRIVE_MODE mode[4];
        double target[4]; //! speeds in radians/s when in MODE_SPEED, [0, 1] when in MODE_PWM
        double offsets[4]; //! offsets [-1, 1] where 0 is the double_stance and +/-1 vertical stance
    bool sync;        // Synchronize wheels

    #ifndef __orogen
        FourWheelCommand()
        {
            for (int i = 0; i < 4; ++i)
            {
                mode[i] = DM_PWM;
                target[i] = 0;
            }
        sync = false;
        }
    #endif
    };

}

#endif /* CONTROLDEVTYPES_HPP */
