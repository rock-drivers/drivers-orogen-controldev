#ifndef CONTROLDEVTYPES_HPP
#define CONTROLDEVTYPES_HPP

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

    /** A unified motion control data structure for differential drive-based
     *  robots.
     */
    struct MotionCommand
    {
        double translation; //! translation value in m/s
        double rotation;    //! rotation value in rad/s. Positive is counter-clockwise
        // That's actually enough information for a differential drive,
        // for an omnidirectional drive we would add a heading/angle
        // value.
    };

    enum FOUR_WHEEL_MODE {
        MODE_PWM, MODE_SPEED
    };

    /** Data structure to send separate commands for each motor */
    struct FourWheelCommand
    {
        FOUR_WHEEL_MODE mode[4];
        double target[4]; //! speeds in radians/s when in MODE_SPEED, [0, 1] when in MODE_PWM

#ifndef __orogen
        FourWheelCommand()
        {
            for (int i = 0; i < 4; ++i)
            {
                mode[i] = MODE_PWM;
                target[i] = 0;
            }
        }
#endif
    };
}

#endif /* CONTROLDEVTYPES_HPP */
