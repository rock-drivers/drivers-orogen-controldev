#include "Remote.hpp"

using namespace controldev;

Remote::Remote(std::string const& name) :
    RemoteBase(name)
{}

/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See Remote.hpp for more detailed
// documentation about them.

bool Remote::configureHook()
{
    return true;
}
// bool Remote::startHook() { return true; }

// void Remote::updateHook() {}
void Remote::updateHook(std::vector<RTT::PortInterface*> const& updated_ports)
{
    MotionCommand mcmd;
    memset(&mcmd, 0, sizeof(MotionCommand));

    RawCommand rcmd;
    memset(&rcmd, 0, sizeof(RawCommand));

    for (std::vector<RTT::PortInterface *>::const_iterator itr = updated_ports.begin();
         itr != updated_ports.end(); itr++)
    {
        if (&_canJoystick == *itr)
        {
            rcmd.devices |= DAI_Joystick;

            can::Message msg;
            this->_canJoystick.read(msg);

            rcmd.joyLeftRight   = ((char)msg.data[0]) / 127.0;
            rcmd.joyFwdBack     = ((char)msg.data[1]) / 127.0;
            rcmd.joyRotation    = ((char)msg.data[2]) / 127.0;
            rcmd.joyThrottle    = -((char)msg.data[3]) / 127.0;
            rcmd.joyButtonCount = 16;
            rcmd.joyButtons     = msg.data[6];

            // [ticks/ms]
            float max_speed = _maxSpeed.get();
            float min_speed = _minSpeed.get();
            float max_speed_ratio = (rcmd.joyThrottle + min_speed) / (1.0 + min_speed);
            double x = rcmd.joyFwdBack   * max_speed * max_speed_ratio;
            double y = rcmd.joyLeftRight * 23328 / 1000.0;
            
            mcmd.rotation    = atan2(y, fabs(x));
            mcmd.translation = x;

            // Send motion command
            this->_motionCommand.write(mcmd);

            // "Only" up to 16 buttons are supported
//            int buttonCount = this->joystick->getNrButtons();
//            buttonCount = (buttonCount > 16 ? 16 : buttonCount);


            // Set button bit list
//            for (int i = 0; i < buttonCount; i++)
//            {
//                if (this->joystick->getButtonPressed(i))
//                {
//                    rcmd.joyButtons |= (1 << i);
//                }
//            }

        }
        else if (&_canSliderBox == *itr)
        {
            can::Message msg;
            this->_canSliderBox.read(msg);

//            rcmd.devices |= DAI_SliderBox;

//            for (int i = 0; i < 7; i++)
//            {
//                rcmd.sliderValues[i] = this->sliderBox->getValue(i);
//            }

//            for (int i = 0; i < 4; i++)
//            {
//                if (this->sliderBox->getButtonOn(i))
//                {
//                    rcmd.sliderButtons |= (1 << i);
//                }
//            }
        }
    }

    // Send raw command
    _rawCommand.write(rcmd);
}


// void Remote::errorHook() {}
// void Remote::stopHook() {}
// void Remote::cleanupHook() {}
