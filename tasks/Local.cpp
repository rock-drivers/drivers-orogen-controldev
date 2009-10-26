#include "Local.hpp"

using namespace controldev;

Local::Local(std::string const& name) :
    LocalBase(name),
    joystick(NULL), sliderBox(NULL)
{}

Local::~Local()
{
    if (this->joystick) delete this->joystick;
    this->joystick = NULL;

    if (this->sliderBox) delete this->sliderBox;
    this->sliderBox = NULL;
}

RTT::FileDescriptorActivity* Local::getFileDescriptorActivity()
{
    return dynamic_cast< RTT::FileDescriptorActivity* >(getActivity().get());
}

/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See Local.hpp for more detailed
// documentation about them.

bool Local::configureHook()
{
    RTT::FileDescriptorActivity *activity = getFileDescriptorActivity();

    if (activity == NULL)
    {
        std::cerr << "Error: Unable to get fd activity, abotring!" << std::endl;
        return false;
    }

    std::string device = this->_joystick_device.value();

    // Try to connect the Joystick
    this->joystick = new Joystick();
    if (!this->joystick->init(device))
    {
        std::cerr << "Warning: Unable to open Joystick device "
            << device << ", Joystick disabled!" << std::endl;

        delete this->joystick;
        this->joystick = NULL;
    }
    else
    {
        activity->watch(this->joystick->getFileDescriptor());
    }

    // Try to connect the SliderBox
    this->sliderBox = new SliderBox();
    if (!this->sliderBox->init())
    {
        std::cerr << "Warning: Unable to open SliderBox device, "
            << "SliderBox disabled!" << std::endl;

        delete this->sliderBox;
        this->sliderBox = NULL;
    }
    else
    {
        this->sliderBox->connectBox();
        activity->watch(this->sliderBox->getFileDescriptor());
    }

    // Abort if no control device was found
    if ((!this->joystick) && (!this->sliderBox))
    {
        std::cerr << "Error: No control device attached, aborting!" << std::endl;
        return false;
    }

    return true;
}
// bool Local::startHook() { return true; }

void Local::updateHook()
{
    MotionCommand mcmd;
    memset(&mcmd, 0, sizeof(MotionCommand));

    RawCommand rcmd;
    memset(&rcmd, 0, sizeof(RawCommand));

    RTT::FileDescriptorActivity *activity = getFileDescriptorActivity();

    // New data available at the Joystick device
    if (joystick && activity->isWatched(this->joystick->getFileDescriptor()))
    {
        this->joystick->updateState();

        rcmd.devices |= (int)DAI_Joystick;

        rcmd.joyLeftRight = this->joystick->getAxis(Joystick::AXIS_Sideward);
        rcmd.joyFwdBack = this->joystick->getAxis(Joystick::AXIS_Forward);
        rcmd.joyRotation = this->joystick->getAxis(Joystick::AXIS_Pan);
        rcmd.joyThrottle = 0.0;

        // Simple transformation of joystick movement to
        // motion commands (translation, rotation)
        double x = (double)rcmd.joyFwdBack;
        double y = (double)rcmd.joyLeftRight;

        mcmd.rotation = atan2(y, x);
        mcmd.translation = ((x != 0 || y != 0) ? sqrt(x * x + y * y) : 0);

        // Send motion command
        this->_motionCommand.write(mcmd);

        // "Only" up to 16 buttons are supported
        int buttonCount = this->joystick->getNrButtons();
        buttonCount = (buttonCount > 16 ? 16 : buttonCount);

        rcmd.joyButtonCount = buttonCount;

        // Set button bit list
        for (int i = 0; i < buttonCount; i++)
        {
            if (this->joystick->getButtonPressed(i))
            {
                rcmd.joyButtons |= (1 << i);
            }
        }
    }

    if (sliderBox && activity->isWatched(this->sliderBox->getFileDescriptor()))
    {
        this->sliderBox->pollNonBlocking();

        rcmd.devices |= DAI_SliderBox;

        for (int i = 0; i < 7; i++)
        {
            rcmd.sliderValues[i] = this->sliderBox->getValue(i);
        }

        for (int i = 0; i < 4; i++)
        {
            if (this->sliderBox->getButtonOn(i))
            {
                rcmd.sliderButtons |= (1 << i);
            }
        }
    }

    // Send raw command
    this->_rawCommand.write(rcmd);
}

// void Local::errorHook() {}
// void Local::stopHook() {}
// void Local::cleanupHook() {}

