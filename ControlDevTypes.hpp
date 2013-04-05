#ifndef CONTROLDEVTYPES_HPP
#define CONTROLDEVTYPES_HPP

#include <base/actuators/commands.h>

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
