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
        double translation;
        double rotation;
        // That's actually enough information for a differential drive,
        // for an omnidirectional drive we would add a heading/angle
        // value.
    };
}

#endif /* CONTROLDEVTYPES_HPP */
