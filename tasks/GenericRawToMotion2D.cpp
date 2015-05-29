/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "GenericRawToMotion2D.hpp"
#include <base/samples/CommandSamples.hpp>

using namespace controldev;

GenericRawToMotion2D::GenericRawToMotion2D(std::string const& name, TaskCore::TaskState initial_state)
    : GenericRawToMotion2DBase(name, initial_state)
{
}

GenericRawToMotion2D::GenericRawToMotion2D(std::string const& name, RTT::ExecutionEngine* engine, TaskCore::TaskState initial_state)
    : GenericRawToMotion2DBase(name, engine, initial_state)
{
}

GenericRawToMotion2D::~GenericRawToMotion2D()
{
}



/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See GenericRawToMotion2D.hpp for more detailed
// documentation about them.

bool GenericRawToMotion2D::configureHook()
{
    if (! GenericRawToMotion2DBase::configureHook())
        return false;
    return true;
}
bool GenericRawToMotion2D::startHook()
{
    if (! GenericRawToMotion2DBase::startHook())
        return false;
    return true;
}
void GenericRawToMotion2D::updateHook()
{
    RawCommand rcmd;
    base::commands::Motion2D  mcmd;
    if(_raw_command.read(rcmd) == RTT::NewData){
        int trans_axis = _translation_axis.get().at(0);
        int trans_subaxis = _translation_axis.get().at(1);

        int rot_axis = _rotation_axis.get().at(0);
        int rot_subaxis = _rotation_axis.get().at(1);
        double trans_raw = rcmd.axisValue[trans_axis][trans_subaxis];
        double rot_raw   = rcmd.axisValue[rot_axis][rot_subaxis];

        mcmd.translation = fabs(trans_raw) < _translation_axis_deadzone ? 0.0 : trans_raw * _maxSpeed;
        double w = (trans_raw < 0.0) ? rot_raw * _maxRotationSpeed : - rot_raw * _maxRotationSpeed;
        mcmd.rotation = fabs(rot_raw) < _rotation_axis_deadzone ? 0.0 : w;
        _motion_command.write(mcmd);
    }

    GenericRawToMotion2DBase::updateHook();
}
void GenericRawToMotion2D::errorHook()
{
    GenericRawToMotion2DBase::errorHook();
}
void GenericRawToMotion2D::stopHook()
{
    GenericRawToMotion2DBase::stopHook();
}
void GenericRawToMotion2D::cleanupHook()
{
    GenericRawToMotion2DBase::cleanupHook();
}
