/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "RawJoystickToMotion2D.hpp"
#include <base/samples/CommandSamples.hpp>

using namespace controldev;

RawJoystickToMotion2D::RawJoystickToMotion2D(std::string const& name, TaskCore::TaskState initial_state)
    : RawJoystickToMotion2DBase(name, initial_state)
{
}

RawJoystickToMotion2D::RawJoystickToMotion2D(std::string const& name, RTT::ExecutionEngine* engine, TaskCore::TaskState initial_state)
    : RawJoystickToMotion2DBase(name, engine, initial_state)
{
}

RawJoystickToMotion2D::~RawJoystickToMotion2D()
{
}

void RawJoystickToMotion2D::sendMotionCommand2D(const RawCommand& rcmd) {
    base::commands::Motion2D mcmd;
    if(rcmd.axisValue.size() != 2) return;
    if(rcmd.axisValue[0].size() != 3) return;
    if(rcmd.axisValue[1].size() != 1) return;

    float max_speed = _maxSpeed.get();
    float min_speed = _minSpeed.get();
    float max_speed_ratio = (rcmd.axisValue[1][0] * min_speed) / (1.0 + min_speed);
    float max_rotation_speed = _maxRotationSpeed.get();
    double x = rcmd.axisValue[0][0] * max_speed * max_speed_ratio;
    double y = rcmd.axisValue[0][1];
    
    mcmd.rotation    = -fabs(y) * atan2(y, fabs(x)) / M_PI * max_rotation_speed;
    mcmd.translation = x;
    
    // Send motion command
    _motion_command.write(mcmd);
}


/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See RawJoystickToMotion2D.hpp for more detailed
// documentation about them.

bool RawJoystickToMotion2D::configureHook()
{
    if (! RawJoystickToMotion2DBase::configureHook())
        return false;
    return true;
}
bool RawJoystickToMotion2D::startHook()
{
    if (! RawJoystickToMotion2DBase::startHook())
        return false;
    return true;
}
void RawJoystickToMotion2D::updateHook()
{
    RawJoystickToMotion2DBase::updateHook();
    while(_raw_command.read(cmd) == RTT::NewData){
        sendMotionCommand2D(cmd);
    }
}
void RawJoystickToMotion2D::errorHook()
{
    RawJoystickToMotion2DBase::errorHook();
}
void RawJoystickToMotion2D::stopHook()
{
    RawJoystickToMotion2DBase::stopHook();
}
void RawJoystickToMotion2D::cleanupHook()
{
    RawJoystickToMotion2DBase::cleanupHook();
}
