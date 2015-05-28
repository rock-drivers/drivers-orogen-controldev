/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "RawWheelToMotion2D.hpp"
#include <base/samples/CommandSamples.hpp>

using namespace controldev;

RawWheelToMotion2D::RawWheelToMotion2D(std::string const& name, TaskCore::TaskState initial_state)
    : RawWheelToMotion2DBase(name, initial_state)
{
}

RawWheelToMotion2D::RawWheelToMotion2D(std::string const& name, RTT::ExecutionEngine* engine, TaskCore::TaskState initial_state)
    : RawWheelToMotion2DBase(name, engine, initial_state)
{
}

RawWheelToMotion2D::~RawWheelToMotion2D()
{
}

void RawWheelToMotion2D::sendMotionCommand2D(const RawCommand& rcmd) {
    base::commands::Motion2D mcmd;
    float max_speed = _maxSpeed.get();
    float min_speed = _minSpeed.get();
    float max_speed_ratio = (rcmd.axisValue[0][3] + min_speed) / (1.0 + min_speed);
    float max_rotation_speed = _maxRotationSpeed.get();
    double x = rcmd.axisValue[0][3]  * max_speed;
    double y = rcmd.axisValue[0][1];

    mcmd.rotation = y;
    mcmd.translation = x;
    
    // Send motion command
    _motion_command.write(mcmd);
}


/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See RawWheelToMotion2D.hpp for more detailed
// documentation about them.

bool RawWheelToMotion2D::configureHook()
{
    if (! RawWheelToMotion2DBase::configureHook())
        return false;
    return true;
}
bool RawWheelToMotion2D::startHook()
{
    if (! RawWheelToMotion2DBase::startHook())
        return false;
    return true;
}
void RawWheelToMotion2D::updateHook()
{
    RawWheelToMotion2DBase::updateHook();
    while(_raw_command.read(cmd) == RTT::NewData){
        sendMotionCommand2D(cmd);
    }
}
void RawWheelToMotion2D::errorHook()
{
    RawWheelToMotion2DBase::errorHook();
}
void RawWheelToMotion2D::stopHook()
{
    RawWheelToMotion2DBase::stopHook();
}
void RawWheelToMotion2D::cleanupHook()
{
    RawWheelToMotion2DBase::cleanupHook();
}
