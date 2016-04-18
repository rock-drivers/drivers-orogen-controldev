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

    mcmd.translation = 0.0;
    mcmd.rotation = 0.0;
    return true;
}
void GenericRawToMotion2D::updateHook()
{
    RawCommand rcmd;
    if (_raw_command.read(rcmd) == RTT::NewData) {

        double trans_raw = rcmd.axisValue[_translation_axis.get().at(0)][_translation_axis.get().at(1)];
        double rot_raw = rcmd.axisValue[_rotation_axis.get().at(0)][_rotation_axis.get().at(1)];

        // just use input values when they exceed deadzone
        trans_raw = fabs(trans_raw) < _translation_axis_deadzone ? 0.0 : trans_raw;
        rot_raw = fabs(rot_raw) < _rotation_axis_deadzone ? 0.0 : rot_raw;


        if (!_acceleration_mode) {
            mcmd.translation = trans_raw * _maxSpeed;
            mcmd.rotation = (trans_raw < 0.0) ? rot_raw * _maxRotationSpeed : - rot_raw * _maxRotationSpeed;
            _motion_command.write(mcmd);
            return;
        }

        /* stop if stop button was pressed */
        int button_cnt = rcmd.buttonValue.size();
        if (_stop_button.get() < button_cnt) {
            uint8_t stop_button = rcmd.buttonValue[_stop_button.get()];
            if (stop_button == 1) {
                mcmd.translation = 0.0;
                mcmd.rotation = 0.0;
                _motion_command.write(mcmd);
                return;
            }
        }

        /* handle the translational part */
        if (_stop_translation_button.get() < button_cnt) {
            uint8_t stop_translation_button = rcmd.buttonValue[_stop_translation_button.get()];
            if (stop_translation_button == 1) {
                mcmd.translation = 0.0;
            } else {
                // just accelerate when input values exceed deadzone
                mcmd.translation += trans_raw * _maxAcceleration;

                // limit translation speed
                mcmd.translation = (mcmd.translation < 0.0) ? std::max(-_maxSpeed.get(), mcmd.translation) : std::min(_maxSpeed.get(), mcmd.translation);
            }
        }

        /* handle the rotational part */
        if (_stop_rotation_button.get() < button_cnt) {
            uint8_t stop_rotation_button = rcmd.buttonValue[_stop_rotation_button.get()];
            if (stop_rotation_button == 1) {
                mcmd.rotation = 0.0;
            } else {
                // just accelerate when input values exceed deadzone
                double acc = rot_raw * _maxRotationAcceleration.get();

                // reverse rotation acceleration when going backwards
                mcmd.rotation = (trans_raw < 0.0) ? mcmd.rotation + acc : mcmd.rotation - acc;

                // limit rotation speed
                mcmd.rotation = mcmd.rotation < 0.0 ? std::max(mcmd.rotation, -_maxRotationSpeed.get()) : std::min(mcmd.rotation, _maxRotationSpeed.get());
            }
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
