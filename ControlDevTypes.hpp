#ifndef CONTROLDEVTYPES_HPP
#define CONTROLDEVTYPES_HPP

#include <base/actuators/commands.h>

namespace controldev
{

    enum DeviceActiveIdentifier
    {
        DAI_None = 0,
        DAI_Joystick = 1,
        DAI_SliderBox = 2
    };

    /** A data structure for raw data values of input devices.
     *  Currently, only the Joystick and SliderBox inmput devices
     *  are supported.
     */
    struct RawCommand
    {
        int devices;

        float joyLeftRight; // Left/right axis in [-1; 1]. Right is positive.
        float joyFwdBack;   // Forward/backward axis in [-1; 1]. Forward is positive.
        float joyRotation;  // Rotation axis in [-1; 1]; Clockwise is positive.
        float joyThrottle;  // Throttle values in [0, 1]. Zero is fully pushed.
        unsigned short joyButtonCount;  // Number of buttons
        unsigned short joyButtons;      // Bitmask showing pressed buttons

        float sliderValues[7]; // The slider box has seven slider values
        unsigned short sliderButtons;    // The slider box has four buttons
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
