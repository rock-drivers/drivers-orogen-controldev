/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "GenericRawToMotion2D.hpp"
#include <base/samples/CommandSamples.hpp>
#include <base/Angle.hpp>

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

void GenericRawToMotion2D::updateControlMode(const ButtonMapping& button_mapping, const std::vector< uint8_t >& buttons)
{
    control_mode = _control_mode.value();

    if(button_mapping.control_off >= 0 &&
        button_mapping.control_off < buttons.size() &&
        buttons[button_mapping.control_off] > 0)
    {
        control_mode = controldev::ControlOff;
    }
    else if(button_mapping.control >= 0 &&
        button_mapping.control < buttons.size() &&
        buttons[button_mapping.control] > 0)
    {
        control_mode = controldev::Control;
    }
    else if(button_mapping.keep_alive >= 0 &&
        button_mapping.keep_alive < buttons.size() &&
        buttons[button_mapping.keep_alive] > 0)
    {
        control_mode = controldev::KeepAlive;
    }

    _control_mode.set(control_mode);
}

/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See GenericRawToMotion2D.hpp for more detailed
// documentation about them.

bool GenericRawToMotion2D::configureHook()
{
    if (! GenericRawToMotion2DBase::configureHook())
        return false;

    control_mode = _control_mode.value();

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
        updateControlMode(_botton_mapping.value(), rcmd.buttonValue);

        if(control_mode == controldev::Control)
        {
            double head_X_raw = rcmd.axisValue[_heading_axis_x.get()];
            double head_Y_raw = rcmd.axisValue[_heading_axis_y.get()];
            double rot_raw    = rcmd.axisValue[_rotation_axis.get()];
            double trans_raw  = rcmd.axisValue[_translation_axis.get()];

            rot_raw = fabs(rot_raw) < _rotation_axis_deadzone ? 0 : rot_raw;
            head_X_raw = fabs(head_X_raw) < _heading_axis_x_deadzone ? 0 : head_X_raw;
            head_Y_raw = fabs(head_Y_raw) < _heading_axis_y_deadzone ? 0 : head_Y_raw;
            trans_raw = fabs(trans_raw) < _translation_axis_deadzone ? 0 : trans_raw;


            rot_raw  *= _invert_rotation_axis ? -1 : 1;
            head_X_raw *= _invert_heading_axis_x ? -1 : 1;
            head_Y_raw *= _invert_heading_axis_y ? -1 : 1;
            trans_raw *= _invert_translation_axis ? -1 : 1;


            mcmd.translation = trans_raw * _maxSpeed;

            mcmd.heading = head_Y_raw == 0 && head_X_raw == 0 ? base::Angle::fromRad(0) : base::Angle::fromRad(atan2(head_X_raw, head_Y_raw));

            mcmd.rotation = rot_raw * _maxRotationSpeed;

            _motion_command.write(mcmd);
        }
        else if(control_mode == controldev::ControlOff)
        {
            mcmd.translation = 0;
            mcmd.rotation = 0;
            _motion_command.write(mcmd);
        }
        else if(control_mode == controldev::KeepAlive)
        {
            mcmd.translation = base::NaN<double>();
            mcmd.rotation = base::NaN<double>();
            _motion_command.write(mcmd);
        }
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
