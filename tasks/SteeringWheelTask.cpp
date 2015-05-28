/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "SteeringWheelTask.hpp"
#include <controldev/LogitechG27.hpp>
#include <rtt/extras/FileDescriptorActivity.hpp>
#include <unistd.h>
#include <fcntl.h>
#include <base/samples/CommandSamples.hpp>

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
    
    base::commands::Motion2D mcmd;
    
    RawCommand rcmd;
    
    bool update = false;

    while(steerControl->updateState())
    {
	update = true;
    }
    
    if(!update)
	return;

    rcmd.deviceIdentifier = steerControl->getName();

    rcmd.axisValue.resize(1);
    rcmd.axisValue[0].resize(5);

    rcmd.axisValue[0][1] = steerControl->getAxis(LogitechG27::AXIS_Wheel);
    rcmd.axisValue[0][0] = steerControl->getAxis(LogitechG27::AXIS_Clutchdirupdown);
    rcmd.axisValue[0][2] = steerControl->getAxis(LogitechG27::AXIS_Clutchdirleftright); 
    rcmd.axisValue[0][3] = steerControl->getAxis(LogitechG27::AXIS_Throttle);
    rcmd.axisValue[0][4] = steerControl->getAxis(LogitechG27::AXIS_Brake);


    int buttonCount = steerControl->getNrButtons();
    
    // Set button bit list
    for (int i = 0; i < buttonCount; i++)
    {
        rcmd.buttonValue.push_back(steerControl->getButtonPressed(i));
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
