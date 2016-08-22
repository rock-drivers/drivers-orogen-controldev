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


bool JoystickTask::updateRawCommand(RawCommand& rcmd) {
    bool update = false;
    // New data available at the Joystick device
    while(this->joystick->updateState())
    {
	update = true;
    }
    
//    if(!update)
//	return false;
    
    rcmd.deviceIdentifier= this->joystick->getName();
    
    rcmd.axisValue.clear();
    for(size_t i = 0; i < joystick->getNrAxis(); i++)
    {
        rcmd.axisValue.push_back(joystick->getAxis(i));
    }
    
    int buttonCount = this->joystick->getNrButtons();

    // Set button bit list
    for (int i = 0; i < buttonCount; i++)
    {
        rcmd.buttonValue.push_back(this->joystick->getButtonPressed(i));
    }
    
    rcmd.time = base::Time::now();

    _raw_command.write(rcmd);

    return true;
}


void JoystickTask::updateHook()
{
    JoystickTaskBase::updateHook();
     
    RawCommand rcmd;
    updateRawCommand(rcmd);
}

void JoystickTask::stopHook()
{
    RTT::extras::FileDescriptorActivity* activity =
        getActivity<RTT::extras::FileDescriptorActivity>();
    if(activity)
        activity->clearAllWatches();

    JoystickTaskBase::stopHook();
}
