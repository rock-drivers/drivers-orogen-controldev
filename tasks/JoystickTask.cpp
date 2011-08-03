/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "JoystickTask.hpp"
#include <controldev/Joystick.hpp>
#include <rtt/extras/FileDescriptorActivity.hpp>

using namespace controldev;

JoystickTask::JoystickTask(std::string const& name)
    : JoystickTaskBase(name), joystick(new controldev::Joystick())
{
}

JoystickTask::JoystickTask(std::string const& name, RTT::ExecutionEngine* engine)
    : JoystickTaskBase(name, engine), joystick(new controldev::Joystick())
{
}

JoystickTask::~JoystickTask()
{
    delete joystick;
}

/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See JoystickTask.hpp for more detailed
// documentation about them.

bool JoystickTask::configureHook()
{
    if (! JoystickTaskBase::configureHook())
        return false;
    
    // Try to connect the Joystick
    if (!joystick->init(_device.value()))
    {
        std::cerr << "Warning: Unable to open Joystick device "
            << _device.value() << std::endl;
	return false;
    }

    return true;
}

bool JoystickTask::startHook()
{
    if (! JoystickTaskBase::startHook())
        return false;
    
    RTT::extras::FileDescriptorActivity* activity =
        getActivity<RTT::extras::FileDescriptorActivity>();
    if (activity)
    {
	activity->watch(joystick->getFileDescriptor());
    }
    
    return true;
}
void JoystickTask::updateHook()
{
    JoystickTaskBase::updateHook();
    
    base::MotionCommand2D mcmd;

    RawCommand rcmd;

    bool update = false;
    // New data available at the Joystick device
    while(this->joystick->updateState())
    {
	update = true;
    }
    
    if(!update)
	return;
    
    rcmd.devices = (int)DAI_Joystick;

    rcmd.joyLeftRight = this->joystick->getAxis(Joystick::AXIS_Sideward);
    rcmd.joyFwdBack = this->joystick->getAxis(Joystick::AXIS_Forward);
    rcmd.joyRotation = this->joystick->getAxis(Joystick::AXIS_Turn); // was Pan for iMoby, has to be Turn for cuslam
    rcmd.joyThrottle = this->joystick->getAxis(Joystick::AXIS_Slider);

    float max_speed = _maxSpeed.get();
    float min_speed = _minSpeed.get();
    float max_speed_ratio = (rcmd.joyThrottle + min_speed) / (1.0 + min_speed);
    float max_rotation_speed = _maxRotationSpeed.get();
    double x = rcmd.joyFwdBack   * max_speed * max_speed_ratio;
    double y = rcmd.joyLeftRight;
    
    mcmd.rotation    = -fabs(y) * atan2(y, fabs(x)) / M_PI * max_rotation_speed;
    mcmd.translation = x;
    
    // Send motion command
    _motion_command.write(mcmd);

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
    
    _raw_command.write(rcmd);
}

void JoystickTask::stopHook()
{
    RTT::extras::FileDescriptorActivity* activity =
        getActivity<RTT::extras::FileDescriptorActivity>();
    activity->clearAllWatches();

    JoystickTaskBase::stopHook();
}
