/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "JoystickTask.hpp"
#include <controldev/Joystick.hpp>
#include <controldev/EvdevHelper.hpp>
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


    /** Joystick information **/
    this->axisCount = joystick->getNrAxis();
    this->buttonCount = joystick->getNrButtons();

    /** Get the configuration information. It requires a proper configuration **/
    this->axisScale = _axisScale.get();
    assert(axisScale.size() == static_cast<unsigned int> (this->axisCount));

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

    RawCommand rcmd;

    if (!updateRawCommand(rcmd)) return;

    sendMotionCommand2D(rcmd);

}

void JoystickTask::stopHook()
{
    RTT::extras::FileDescriptorActivity* activity =
        getActivity<RTT::extras::FileDescriptorActivity>();
    if(activity)
        activity->clearAllWatches();

    JoystickTaskBase::stopHook();
}
bool JoystickTask::updateRawCommand(RawCommand& rcmd) {

    bool update = false;

    /** New data available at the Joystick device **/
    while(this->joystick->updateState())
    {
	update = true;
    }

    /** Device name **/
    rcmd.deviceIdentifier= this->joystick->getName();

    /** Axis mapping **/
    rcmd.axes.names = this->joystick->getMapAxis();
    register size_t l = 0;
    std::vector<double> axes_value = this->joystick->getAxes();
    for(std::vector<double>::iterator it=axes_value.begin(); it!=axes_value.end(); ++it)
    {
        rcmd.axes.elements.push_back((*it)*axisScale[l]);
        l++;
    }

    /** Buttons mapping **/
    rcmd.buttons.names = this->joystick->getMapButtons();
    std::vector<bool> buttons_value = this->joystick->getButtons();
    for(std::vector<bool>::iterator it=buttons_value.begin(); it!=buttons_value.end(); ++it)
    {
        rcmd.buttons.elements.push_back(static_cast<int>(*it));
    }

    /** Time stamp **/
    rcmd.time = base::Time::now();

    /** Write in the port **/
    _raw_command.write(rcmd);

    return update;
}

/** This is a simple/generic 2D command generator each subclass should have its own one **/
void JoystickTask::sendMotionCommand2D(const RawCommand& rcmd)
{
    base::MotionCommand2D mcmd;

    /** At least two axes to command a 2D motion **/
    if(rcmd.axes.size() < 2) return;

    /** Configuration values **/
    float max_speed = _maxSpeed.get();
    float min_speed = _minSpeed.get();
    float max_rotation_speed = _maxRotationSpeed.get();

    /** Speed ratio only it has a throttle absolute axis (ABS)**/
    float max_speed_ratio;
    try
    {
        /** In linux/input.h and used by the controldev library defines the ABS_THROTTLE **/
        double throttle = rcmd.axes[abs2str(6)];

        /** Throttle between 0  and 1**/
        max_speed_ratio = 0.5 + (throttle/2.0);

       //What was max_speed ratio before ??
       // max_speed_ratio = (rcmd.axes[abs2str(6)] * min_speed) / (1.0 + min_speed);
    }
    catch (const std::runtime_error& error)
    {
        /** TO-DO: perhaps write in the RTT log that there is not a throttle axis **/
        max_speed_ratio = 1.0;
    }

    /** Get the x-y information. TO-DO: it could also be accessed by name **/
    double x = rcmd.axes[1] * max_speed * max_speed_ratio;
    double y = rcmd.axes[0];

    /** Calculate the 2D command **/
    mcmd.rotation    = -fabs(y) * atan2(y, fabs(x)) / M_PI * max_rotation_speed;
    mcmd.translation = x;

    /** Send motion command **/
    _motion_command.write(mcmd);
}


