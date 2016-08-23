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

int JoystickTask::getFileDescriptor()
{
    return joystick->getFileDescriptor();
}

bool JoystickTask::updateRawCommand(RawCommand& rcmd) {
    // New data available at the Joystick device
    while(joystick->updateState())
    {
    }
    
    rcmd.deviceIdentifier= joystick->getName();
    
    rcmd.axisValue = joystick->getAxes();
    
    size_t buttonCount = joystick->getNrButtons();

    // Set button bit list
    for (size_t i = 0; i < buttonCount; i++)
    {
        rcmd.buttonValue.push_back(joystick->getButtonPressed(i));
    }
    
    rcmd.time = base::Time::now();

    return true;
}


void JoystickTask::updateHook()
{
    JoystickTaskBase::updateHook();
     
    RawCommand rcmd;
    updateRawCommand(rcmd);
    _raw_command.write(rcmd);
}

