#include "Local.hpp"

#include <rtt/extras/FileDescriptorActivity.hpp>

using namespace controldev;

Local::Local(std::string const& name) :
    LocalBase(name),
    joystick(NULL), sliderBox(NULL)
{
    _maxSpeed.set(1.5);
    _minSpeed.set(0.1);
    _maxRotationSpeed.set(M_PI);
}

Local::~Local()
{
    if (this->joystick) delete this->joystick;
    this->joystick = NULL;

    if (this->sliderBox) delete this->sliderBox;
    this->sliderBox = NULL;
}

/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See Local.hpp for more detailed
// documentation about them.

bool Local::configureHook()
{
    RTT::extras::FileDescriptorActivity* activity =
        getActivity<RTT::extras::FileDescriptorActivity>();

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
    base::MotionCommand2D mcmd;
    memset(&mcmd, 0, sizeof(base::MotionCommand2D));

    RawCommand rcmd;
    memset(&rcmd, 0, sizeof(RawCommand));

    RTT::extras::FileDescriptorActivity* activity =
        getActivity<RTT::extras::FileDescriptorActivity>();

    // New data available at the Joystick device
    if (joystick && activity->isWatched(this->joystick->getFileDescriptor()))
    {
        while(this->joystick->updateState());

        rcmd.devices |= (int)DAI_Joystick;

        rcmd.joyLeftRight = this->joystick->getAxis(Joystick::AXIS_Sideward);
        rcmd.joyFwdBack = this->joystick->getAxis(Joystick::AXIS_Forward);
        rcmd.joyRotation = this->joystick->getAxis(Joystick::AXIS_Pan);
        rcmd.joyThrottle = this->joystick->getAxis(Joystick::AXIS_Slider);

        // Simple transformation of joystick movement to
        // motion commands (translation, rotation)
//         double x = (double)rcmd.joyFwdBack;
//         double y = (double)rcmd.joyLeftRight;
// 
//         mcmd.rotation = atan2(y, x);
//         mcmd.translation = ((x != 0 || y != 0) ? sqrt(x * x + y * y) : 0);

	float max_speed = _maxSpeed.get();
	float min_speed = _minSpeed.get();
	float max_speed_ratio = (rcmd.joyThrottle + min_speed) / (1.0 + min_speed);
	float max_rotation_speed = _maxRotationSpeed.get();
	double x = rcmd.joyFwdBack   * max_speed * max_speed_ratio;
	double y = rcmd.joyLeftRight;
	
	mcmd.rotation    = -fabs(y) * atan2(y, fabs(x)) / M_PI * max_rotation_speed;
	mcmd.translation = x;
	
        // Send motion command
        this->_motion_command.write(mcmd);

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
	bool updated = false;
        while(this->sliderBox->pollNonBlocking(updated));

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
    this->_raw_command.write(rcmd);
}

// void Local::errorHook() {}
// void Local::stopHook() {}
// void Local::cleanupHook() {}

