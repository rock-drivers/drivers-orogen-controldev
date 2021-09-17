/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "GenericRawToTwist.hpp"
#include <base/Twist.hpp>
#include <base/Angle.hpp>


#define WRITEAXIS(AXISNAME, TARGET) {\
        int axis = _##AXISNAME.get(); \
        if (axis >= 0) { \
            double axisvalue = rcmd.axisValue[axis]; \
            axisvalue = fabs(axisvalue) < _##AXISNAME##_deadzone ? 0 : axisvalue; \
            axisvalue  *= _invert_##AXISNAME ? -1 : 1; \
            TARGET = axisvalue * _max_##AXISNAME; \
        } else { \
            TARGET = 0; \
        } \
    };


using namespace controldev;

GenericRawToTwist::GenericRawToTwist(std::string const& name, TaskCore::TaskState initial_state)
    : GenericRawToTwistBase(name, initial_state)
{
}

GenericRawToTwist::~GenericRawToTwist()
{
}



/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See GenericRawToTwist.hpp for more detailed
// documentation about them.

bool GenericRawToTwist::configureHook()
{
    if (! GenericRawToTwistBase::configureHook())
        return false;
    return true;
}
bool GenericRawToTwist::startHook()
{
    if (! GenericRawToTwistBase::startHook())
        return false;
    return true;
}


void GenericRawToTwist::updateHook()
{
    GenericRawToTwistBase::updateHook();

    RawCommand rcmd;
    base::Twist mcmd;
    if (_raw_command.read(rcmd) == RTT::NewData) {
        // example code result by the WRITEAXIS macro
        // int axis = _linear_x_axis.get();
        // if (axis >= 0) {
        //     double axisvalue = rcmd.axisValue[axis];
        //     axisvalue = fabs(rot_raw) < _rotation_axis_deadzone ? 0 : rot_raw;
        //     axisvalue  *= _invert_linear_x_axis ? -1 : 1;
        //     mcmd.linear.x() = axisvalue * _max_linear_x_axis;
        // } else {
        //     mcmd.linear.x() = 0;
        // }

        WRITEAXIS(linear_x_axis, mcmd.linear.x())
        WRITEAXIS(linear_y_axis, mcmd.linear.y())
        WRITEAXIS(linear_z_axis, mcmd.linear.z())
        WRITEAXIS(angular_x_axis, mcmd.angular.x())
        WRITEAXIS(angular_y_axis, mcmd.angular.y())
        WRITEAXIS(angular_z_axis, mcmd.angular.z())
        _motion_command.write(mcmd);
    }
}
void GenericRawToTwist::errorHook()
{
    GenericRawToTwistBase::errorHook();
}
void GenericRawToTwist::stopHook()
{
    GenericRawToTwistBase::stopHook();
}
void GenericRawToTwist::cleanupHook()
{
    GenericRawToTwistBase::cleanupHook();
}
