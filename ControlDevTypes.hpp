 //! Data structures for control devices and controllers

namespace controldev
{

    enum DeviceActiveIdentifier
    {
        None = 0,
        Joystick = 1,
        SliderBox = 2,
    };

    /** A data structure for raw data values of input devices.
     *  Currently, only the Joystick and SliderBox inmput devices
     *  are supported.
     */
    struct RawCommand
    {
        DeviceActiveIdentifier devices;

        short joyLeftRight;           // The Linux API returns 16bit values
        short joyUpDown;              // The Linux API returns 16bit values
        short joySlide;               // not applicable for ASGuard
        unsigned short joyThrottle;   // Throttle values are positive
        unsigned short joyButtons;    // 16 buttons should be enough

        short sliderValues[7];        // The slider box has seven slider values
        unsigned char slilderButtons; // The slider box has four buttons
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

