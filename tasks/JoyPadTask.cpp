/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "JoyPadTask.hpp"

using namespace controldev;

JoyPadTask::JoyPadTask(std::string const& name)
    : JoyPadTaskBase(name), joystick(new controldev::Joystick())
{
}

JoyPadTask::JoyPadTask(std::string const& name, RTT::ExecutionEngine* engine)
    : JoyPadTaskBase(name, engine)
{
}

JoyPadTask::~JoyPadTask()
{
    delete joystick;
}



/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See JoyPadTask.hpp for more detailed
// documentation about them.

bool JoyPadTask::configureHook()
{
    if (! JoyPadTaskBase::configureHook())
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
bool JoyPadTask::startHook()
{
    if (! JoyPadTaskBase::startHook())
        return false;

    RTT::extras::FileDescriptorActivity* activity =
        getActivity<RTT::extras::FileDescriptorActivity>();
    if (activity)
    {
	activity->watch(joystick->getFileDescriptor());
    }

    rcmd.deviceIdentifier= joystick->getName();

    rcmd.axisValue.resize(1);
    rcmd.axisValue[0].resize(joystick->getNrAxis());
    rcmd.buttonValue.resize(joystick->getNrButtons());
    
    return true;
}
void JoyPadTask::updateHook()
{
    JoyPadTaskBase::updateHook();
    // New data available at the Joystick device
    while( joystick->updateState() );
    
    // Set axes values
    for (unsigned int i = 0; i < joystick->getNrAxis(); ++i)
    {
        rcmd.axisValue[0][i] = joystick->getAxis( Joystick::Axis(i) );
    }
    // Set button bit list
    for (unsigned int i = 0; i < joystick->getNrButtons(); ++i)
    {
        rcmd.buttonValue[i] = joystick->getButtonPressed(i);
    }
    
    rcmd.time = base::Time::now();

    _raw_command.write(rcmd);
}
void JoyPadTask::errorHook()
{
    JoyPadTaskBase::errorHook();
}
void JoyPadTask::stopHook()
{
    RTT::extras::FileDescriptorActivity* activity =
        getActivity<RTT::extras::FileDescriptorActivity>();
    if(activity)
        activity->clearAllWatches();
    JoyPadTaskBase::stopHook();
}
void JoyPadTask::cleanupHook()
{
    JoyPadTaskBase::cleanupHook();
}
