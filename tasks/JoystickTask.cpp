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

int JoystickTask::getFileDescriptor()
{
    return joystick->getFileDescriptor();
}

bool JoystickTask::configureHook()
{
    if (! JoystickTaskBase::configureHook())
        return false;

    connectDevice();
    return true;
}

void JoystickTask::connectDevice(bool recover)
{
    // Try to connect the Joystick
    if (recover)
    {
        if (not recoverConnection())
        {
            return;
        }
    }

    if (joystick->init(_device.value()))
    {
        if (! device_connected_)
        {
            device_connected_ = true;
            std::cout << "INFO: Device " << _device.value() << " connected" << std::endl;
        }
    } else {
        if (device_connected_)
        {
            std::cerr << "Warning: Unable to open Joystick device "
                << _device.value() << std::endl;
            device_connected_ = false;
        }
    }
}

bool JoystickTask::recoverConnection()
{
    delete joystick;
    // from generic stop hook
    RTT::extras::FileDescriptorActivity* activity =
        getActivity<RTT::extras::FileDescriptorActivity>();
    if(activity)
        activity->clearAllWatches();

    joystick = new controldev::Joystick();
    joystick->init(_device.value());
    int fdID = joystick->getFileDescriptor();

    // from generic start hook
    if (activity && fdID != -1)
    {
        activity->watch(getFileDescriptor());
        //get trigger a least every 25 ms
        activity->setTimeout(25);
        return true;
    } else {
        return false;
    }
}

void JoystickTask::updateHook()
{
    if (device_connected_)
    {
        state(CONNECTED);
        JoystickTaskBase::updateHook();
        writeCommand();
    } else {
        state(NOT_CONNECTED);
        connectDevice(true);
    }
}

void JoystickTask::writeCommand()
{
    RawCommand rcmd;
    updateRawCommand(rcmd);
    _raw_command.write(rcmd);
}

bool JoystickTask::updateRawCommand(RawCommand& rcmd) {
    // New data available at the Joystick device
    while(joystick->updateState())
    {
    }

    rcmd.deviceIdentifier= joystick->getName();
    rcmd.axisValue = joystick->getAxes();
    if (not checkAxisValues(rcmd) )
    {
        throw std::runtime_error("Invalid axis values. Try to forget device and reconnect");
    }

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
        device_connected_ = false;
        state(NOT_CONNECTED);
        return false;
    }
    
    rcmd.time = base::Time::now();
    return true;
}

bool JoystickTask::checkAxisValues(const RawCommand& rmcd)
{
    unsigned int counter = 0;    
    for (const auto & value : rmcd.axisValue)
    {
        if (value == -1)
        {
            counter++;
        }
    }
    return counter < 5;
}
