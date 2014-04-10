#ifndef CONTROLDEVTYPES_HPP
#define CONTROLDEVTYPES_HPP

#include <base/actuators/commands.h>
#include <base/NamedVector.hpp>

namespace controldev
{

    enum DeviceActiveIdentifier
    {
        DAI_None = 0,
        DAI_Joystick = 1,
        DAI_SliderBox = 2,
	DAI_SteeringWheel = 3
    };

    enum Axis{
        RX=0,
        RY,
        RZ,
        TX,
        TY,
        TZ
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
        base::actuators::DRIVE_MODE mode[4];
        double target[4]; //! speeds in radians/s when in MODE_SPEED, [0, 1] when in MODE_PWM
        double offsets[4]; //! offsets [-1, 1] where 0 is the double_stance and +/-1 vertical stance
	bool sync;        // Synchronize wheels

#ifndef __orogen
        FourWheelCommand()
        {
            for (int i = 0; i < 4; ++i)
            {
                mode[i] = base::actuators::DM_PWM;
                target[i] = 0;
            }
	    sync = false;
        }
#endif
    };
}

#endif /* CONTROLDEVTYPES_HPP */
