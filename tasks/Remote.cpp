#include "Remote.hpp"

using namespace controldev;

Remote::Remote(std::string const& name) :
    RemoteBase(name)
{
    // Set max/min speed to respectively 0.5 m/s and 0.1 m/s
    _maxSpeed.set(1.5);
    _minSpeed.set(0.1);
    _maxRotationSpeed.set(M_PI);
}

/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See Remote.hpp for more detailed
// documentation about them.

bool Remote::configureHook()
{
    return true;
}
// bool Remote::startHook() { return true; }

void Remote::updateHook()
{
    base::MotionCommand2D mcmd;
    RawCommand rcmd;

    canbus::Message msg;

    bool gotCmd = false;
    while (_canJoystick.read(msg) == RTT::NewData)
    {
	if(msg.can_id != 0x100)
	    continue;

	gotCmd = true;

        rcmd.deviceIdentifier = "CAN-Joystick";

        rcmd.axisValue.resize(3);
        rcmd.axisValue[0].resize(3);
        rcmd.axisValue[1].resize(1);
        rcmd.axisValue[2].resize(2);

        rcmd.axisValue[0][1]    = ((char)msg.data[0]) / 127.0;
        rcmd.axisValue[0][0]    = ((char)msg.data[1]) / 127.0;
        rcmd.axisValue[0][2]    = ((char)msg.data[2]) / 127.0;
        rcmd.axisValue[1][0]    = -((char)msg.data[3]) / 127.0;
        rcmd.axisValue[2][0]    = ((char)msg.data[4]) / 127.0;
        rcmd.axisValue[2][1]    = ((char)msg.data[5]) / 127.0;
        for(int i=0;i<8;i++){
            rcmd.buttonValue.push_back(msg.data[6] &(1<<i));
        }

        // [ticks/ms]
        float max_speed = _maxSpeed.get();
        float min_speed = _minSpeed.get();
        float max_speed_ratio = (rcmd.axisValue[1][0] + min_speed) / (1.0 + min_speed);
        float max_rotation_speed = _maxRotationSpeed.get();
        double x = rcmd.axisValue[0][0]   * max_speed * max_speed_ratio;
        double y = rcmd.axisValue[0][1];
        
        mcmd.rotation    = -fabs(y) * atan2(y, fabs(x)) / M_PI * max_rotation_speed;
        mcmd.translation = x;

    }

    if(gotCmd)
    {
        // Send motion command
        _motion_command.write(mcmd);
        
        // Send raw command
        _raw_command.write(rcmd);
    }

    while (_canSliderBox.read(msg) == RTT::NewData)
    {
        rcmd.axisValue.clear();
        rcmd.buttonValue.clear();

        rcmd.axisValue.push_back(std::vector<double>());
        rcmd.axisValue[0].resize(6);
        rcmd.deviceIdentifier = "CAN-Sliderbox";

        for (int i = 0; i < 7; i++)
        {
            rcmd.axisValue[0][i] = msg.data[i];
        }

        for(int i=0;i<8;i++){
            rcmd.buttonValue.push_back(msg.data[7] &(1<<i));
        }

        FourWheelCommand wheel_command;
        if (mapFromSliderbox(wheel_command, rcmd))
            _four_wheel_command.write(wheel_command);

        // Send raw command
        _raw_command.write(rcmd);
    }

}


// void Remote::errorHook() {}
// void Remote::stopHook() {}
// void Remote::cleanupHook() {}

