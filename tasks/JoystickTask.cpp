/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "JoystickTask.hpp"
#include <controldev/Joystick.hpp>
#include <rtt/extras/FileDescriptorActivity.hpp>

using namespace controldev;

JoystickTask::JoystickTask(std::string const& name)
    : JoystickTaskBase(name), joystick(new controldev::Joystick()), device_connected_(false)
{
}

JoystickTask::JoystickTask(std::string const& name, RTT::ExecutionEngine* engine)
    : JoystickTaskBase(name, engine), joystick(new controldev::Joystick()), device_connected_(false)
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

    connectDevice();

    return true;
}

int JoystickTask::getFileDescriptor()
{
//    int i;
//    try
//    {
//        i = joystick->getFileDescriptor();
//    } catch (const std::exception & e) {
//        std::cerr << "ERROR caught myself! " << std::endl;
//        std::cerr << "ERROR caught: " << e.what() << std::endl;
//    }
    return joystick->getFileDescriptor();
//    return i;

}

bool JoystickTask::updateRawCommand(RawCommand& rcmd) {
    // New data available at the Joystick device
    while(joystick->updateState())
    {
    }

    rcmd.deviceIdentifier= joystick->getName();
    
    rcmd.axisValue = joystick->getAxes();
    
    size_t buttonCount = joystick->getNrButtons();

    try
    {
        // Set button bit list
        for (size_t i = 0; i < buttonCount; i++)
        {
            rcmd.buttonValue.push_back(joystick->getButtonPressed(i));
        }
    } catch (const std::runtime_error& e) {
        std::cerr << "Warning: Caught runtime error. Lost connection to device." << std::endl;
        delete joystick;
        joystick = new controldev::Joystick();
        device_connected_ = false;
        state(NOT_CONNECTED);
        return false;
    }
    
    rcmd.time = base::Time::now();

    return true;
}


void JoystickTask::updateHook()
{
    std::cout << "DEBUG: in updateHook: " << __LINE__ << std::endl;
    if (device_connected_)
    {
        state(CONNECTED);
        JoystickTaskBase::updateHook();

        RawCommand rcmd;
        updateRawCommand(rcmd);
        _raw_command.write(rcmd);
    } else {
        state(NOT_CONNECTED);
        connectDevice();
    }
}

bool JoystickTask::connectDevice()
{
    // Try to connect the Joystick
    std::cout << "DEBUG: in connectDevice: " << __LINE__ << std::endl;
    if (joystick->init(_device.value()))
    {
        device_connected_ = true;
        std::cout << "INFO: Device " << _device.value() << " connected" << std::endl;
        return true;
    } else {
        if (device_connected_)
        {
            std::cerr << "Warning: Unable to open Joystick device "
                << _device.value() << std::endl;
        }
        device_connected_ = false;
        return false;
    }
}
