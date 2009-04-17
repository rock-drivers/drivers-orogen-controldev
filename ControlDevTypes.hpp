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

        short joyLeftRight;             // The Linux API returns 16bit values
        short joyUpDown;                // The Linux API returns 16bit values
        short joySlide;                 // not applicable for ASGuard
        unsigned short joyThrottle;     // Throttle values are positive
        unsigned short joyButtonCount;  // Number of buttons
        unsigned short joyButtons;      // 16 buttons should be enough

        unsigned short sliderValues[7]; // The slider box has seven slider values
        unsigned char sliderButtons;    // The slider box has four buttons
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
