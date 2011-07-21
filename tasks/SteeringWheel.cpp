#include "SteeringWheel.hpp"

#include <rtt/extras/FileDescriptorActivity.hpp>

using namespace controldev;

SteeringWheel::SteeringWheel(std::string const& name) :
    SteeringWheelBase(name),
    steerControl(NULL)
{
    _maxSpeed.set(1.5);
    _minSpeed.set(0.1);
    _maxRotationSpeed.set(M_PI);
}

SteeringWheel::~SteeringWheel()
{
    if (this->steerControl) delete this->steerControl;
    this->steerControl = NULL;
}

/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See SteeringWheel.hpp for more detailed
// documentation about them.

bool SteeringWheel::configureHook()
{
    // std::string device = this->_steerControl_device.value();
    // LogitechG27 class does look for the device itself, no need to supply device

    // Try to connect the Wheel
    this->steerControl = new LogitechG27();
    if (!this->steerControl->init())
    {
        std::cerr << "Warning: Unable to open G27 Racing Wheel device "
            << ", Steering wheel disabled!" << std::endl;

        delete this->steerControl;
        this->steerControl = NULL;
    }

    // Abort if no control device was found
    if (!this->steerControl)
    {
        std::cerr << "Error: No control device attached, aborting!" << std::endl;
        return false;
    }

    return SteeringWheelBase::configureHook();
}

bool SteeringWheel::startHook()
{
    if (! SteeringWheelBase::startHook())
        return false;

    RTT::extras::FileDescriptorActivity* activity =
        getActivity<RTT::extras::FileDescriptorActivity>();
    if (activity)
    {
        if (this->steerControl)
            activity->watch(this->steerControl->getFileDescriptor());
    }

    return true;
}


void SteeringWheel::updateHook()
{
    base::MotionCommand2D mcmd;
    memset(&mcmd, 0, sizeof(base::MotionCommand2D));

    RawCommand rcmd;
    memset(&rcmd, 0, sizeof(RawCommand));

    base::SpeedCommand6D scmd;
    memset(&scmd, 0, sizeof(base::SpeedCommand6D));

    RTT::extras::FileDescriptorActivity* activity =
        getActivity<RTT::extras::FileDescriptorActivity>();

    // New data available at the Joystick device
    if (steerControl && (!activity || activity->isUpdated(this->steerControl->getFileDescriptor())))
    {
      //std::cout << "activity" << std::endl;
      this->steerControl->updateState();
	  //std::cout << "state updated" << std::endl;

        rcmd.devices |= (int)DAI_SteeringWheel;

        rcmd.joyLeftRight = this->steerControl->getAxis(LogitechG27::AXIS_Wheel);
        rcmd.joyFwdBack = this->steerControl->getAxis(LogitechG27::AXIS_Clutchdirupdown);
        rcmd.joyRotation = this->steerControl->getAxis(LogitechG27::AXIS_Clutchdirleftright); // was Pan for iMoby, has to be Turn for cuslam
        rcmd.joyThrottle = this->steerControl->getAxis(LogitechG27::AXIS_Throttle);

        scmd.surge = rcmd.joyFwdBack;
	scmd.sway = rcmd.joyLeftRight;
	scmd.heave = rcmd.joyThrottle;

	scmd.roll = 0;
	scmd.pitch = 0;
	scmd.yaw = rcmd.joyRotation;

	this->_speed_command.write(scmd);

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
        int buttonCount = this->steerControl->getNrButtons();
        buttonCount = (buttonCount > 16 ? 16 : buttonCount);

        rcmd.joyButtonCount = buttonCount;

        // Set button bit list
        for (int i = 0; i < buttonCount; i++)
        {
            if (this->steerControl->getButtonPressed(i))
            {
                rcmd.joyButtons |= (1 << i);
            }
        }
    }
    
    // Send raw command
    this->_raw_command.write(rcmd);
}

// void SteeringWheel::errorHook() {}
void SteeringWheel::stopHook()
{
    RTT::extras::FileDescriptorActivity* activity =
        getActivity<RTT::extras::FileDescriptorActivity>();
    activity->clearAllWatches();

    SteeringWheelBase::stopHook();
}
// void SteeringWheel::cleanupHook() {}

