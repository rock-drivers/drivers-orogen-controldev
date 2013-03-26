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
         */
        std::vector<bool> buttonValue;

        /*   DEPRICATED
        int devices;

           float joyLeftRight; // Left/right axis in [-1; 1]. Right is positive.
           float joyFwdBack;   // Forward/backward axis in [-1; 1]. Forward is positive.
           float joyRotation;  // Rotation axis in [-1; 1]; Clockwise is positive.
           float joyThrottle;  // Throttle values in [0, 1]. Zero is fully pushed.
           float joyBrake;     // Brake pedal
           int joyButtonCount;  // Number of buttons
           int32_t joyButtons;      // Bitmask showing pressed buttons
           float additionalAxis[2]; //Additional Axis (e.G. on Logitech XTreme 3D the thumb-direction button)

           float sliderValues[7]; // The slider box has seven slider values
           unsigned short sliderButtons;    // The slider box has four buttons
           
           RawCommand(): joyLeftRight(0), joyFwdBack(0), joyRotation(0), 
                         joyThrottle(0), joyBrake(), joyButtonCount(0), 
                         joyButtons(0), sliderButtons(0) {
                           additionalAxis[0] =0;
                           additionalAxis[1] =0;
                         };
                         */
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
