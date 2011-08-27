/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "SteeringWheelTask.hpp"
#include <controldev/LogitechG27.hpp>
#include <rtt/extras/FileDescriptorActivity.hpp>


using namespace controldev;

SteeringWheelTask::SteeringWheelTask(std::string const& name)
    : SteeringWheelTaskBase(name), steerControl(new controldev::LogitechG27())
{
}

SteeringWheelTask::SteeringWheelTask(std::string const& name, RTT::ExecutionEngine* engine)
    : SteeringWheelTaskBase(name, engine), steerControl(new controldev::LogitechG27())
{
}

SteeringWheelTask::~SteeringWheelTask()
{
    delete steerControl;
}



/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See SteeringWheelTask.hpp for more detailed
// documentation about them.

bool SteeringWheelTask::configureHook()
{
    if (! SteeringWheelTaskBase::configureHook())
        return false;
    
    if (!steerControl->init())
    {
        std::cerr << "Warning: Unable to open G27 Racing Wheel device " << std::endl;
	return false;
    }

    return true;
}

bool SteeringWheelTask::startHook()
{
    if (! SteeringWheelTaskBase::startHook())
        return false;
    
    RTT::extras::FileDescriptorActivity* activity =
        getActivity<RTT::extras::FileDescriptorActivity>();
    if (activity)
    {
	activity->watch(steerControl->getFileDescriptor());
    }

    return true;
}

void SteeringWheelTask::updateHook()
{
    SteeringWheelTaskBase::updateHook();
    
    base::MotionCommand2D mcmd;
    
    RawCommand rcmd;
    
    //note, this method actually blocks and returns true in any case
    steerControl->updateState();

    rcmd.devices |= (int)DAI_SteeringWheel;

    rcmd.joyLeftRight = steerControl->getAxis(LogitechG27::AXIS_Wheel);
    rcmd.joyFwdBack = steerControl->getAxis(LogitechG27::AXIS_Clutchdirupdown);
    rcmd.joyRotation = steerControl->getAxis(LogitechG27::AXIS_Clutchdirleftright); // was Pan for iMoby, has to be Turn for cuslam
    rcmd.joyThrottle = steerControl->getAxis(LogitechG27::AXIS_Throttle);

    float max_speed = _maxSpeed.get();
    float min_speed = _minSpeed.get();
    float max_speed_ratio = (rcmd.joyThrottle + min_speed) / (1.0 + min_speed);
    float max_rotation_speed = _maxRotationSpeed.get();
    double x = rcmd.joyFwdBack   * max_speed * max_speed_ratio;
    double y = rcmd.joyLeftRight;

    // mcmd.rotation    = -fabs(y) * atan2(y, fabs(x)) / M_PI * max_rotation_speed;
    mcmd.rotation = y;
    mcmd.translation = x;
    
    // Send motion command
    _motion_command.write(mcmd);

    // "Only" up to 16 buttons are supported
    int buttonCount = steerControl->getNrButtons();
    buttonCount = (buttonCount > 16 ? 16 : buttonCount);

    rcmd.joyButtonCount = buttonCount;

    // Set button bit list
    for (int i = 0; i < buttonCount; i++)
    {
	if (steerControl->getButtonPressed(i))
	{
	    rcmd.joyButtons |= (1 << i);
	}
    }
    
    // Send raw command
    _raw_command.write(rcmd);
}

void SteeringWheelTask::stopHook()
{
    RTT::extras::FileDescriptorActivity* activity =
        getActivity<RTT::extras::FileDescriptorActivity>();
    activity->clearAllWatches();

    SteeringWheelTaskBase::stopHook();
}
