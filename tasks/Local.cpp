#include "Local.hpp"

using namespace controldev;

Local::Local(std::string const& name, TaskCore::TaskState initial_state) :
    LocalBase(name, initial_state),
    joystick(NULL), sliderBox(NULL)
{}

Local::~Local()
{
    if (this->joystick) delete this->joystick;
    this->joystick = NULL;

    if (this->sliderBox) delete this->sliderBox;
    this->sliderBox = NULL;
}


/** This method is called after the configuration step by the
 * FileDescriptorActivity to get the file descriptor
 */
std::vector<int> Local::getFileDescriptors() const
{
    return std::vector<int>();
}


/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See Local.hpp for more detailed
// documentation about them.

bool Local::configureHook()
{
    std::string device = this->_joystick_device.value();

    // Try to connect the Joystick
    this->joystick = new Joystick();
    if (!this->joystick->init(device))
    {
        std::cerr << "Warning: Unable to open Joystick device "
            << device << ", Joystick disabled!" << std::endl;

        delete this->joystick;
        this->joystick = NULL;
    }

    // Try to connect the SliderBox
    this->sliderBox = new SliderBox();
    if (!this->sliderBox->init())
    {
        std::cerr << "Warning: Unable to open SliderBox device, "
            << "SliderBox disabled!" << std::endl;

        delete this->sliderBox;
        this->sliderBox = NULL;
    }
    this->sliderBox->connectBox();

    // Abort if no control device was found
    if ((!this->joystick) && (!this->sliderBox))
    {
        std::cerr << "Error: No control device attached, aborting!" << std::endl;
        return false;
    }

    return true;
}
// bool Local::startHook() { return true; }

void Local::updateHook()
{
    RawCommand cmd;

    memset(&cmd, 0, sizeof(RawCommand));

    for (size_t i = 0; i < this->__fd_count; i++)
    {
        // New data available at the Joystick device
        if (this->__fds[i] == this->joystick->getFileDescriptor())
        {
            this->joystick->updateState();

            cmd.devices |= (int)DAI_Joystick;
            cmd.joyLeftRight = this->joystick->getAxis(Joystick::AXIS_Sideward);
            cmd.joyUpDown = this->joystick->getAxis(Joystick::AXIS_Forward);
            cmd.joySlide = this->joystick->getAxis(Joystick::AXIS_Pan);
            cmd.joyThrottle = 0.0;

            // "Only" up to 16 buttons are supported
            int buttonCount = this->joystick->getNrButtons();
            buttonCount = (buttonCount > 16 ? 16 : buttonCount);

            cmd.joyButtonCount = buttonCount;

            // Set button bit list
            for (int j = 0; j < buttonCount; j++)
            {
                if (this->joystick->getButtonPressed(j))
                {
                    cmd.joyButtons |= (1 << j);
                }
            }
        }

        // New data available at the SliderBox device
        else if (this->__fds[i] == this->sliderBox->getFileDescriptor())
        {
            this->sliderBox->pollNonBlocking();

            cmd.devices |= DAI_SliderBox;

            for (int j = 0; j < 7; i++)
            {
                cmd.sliderValues[i] = this->sliderBox->getValue(i);
            }

            for (int j = 0; j < 4; i++)
            {
                cmd.sliderButtons |= this->sliderBox->getButtonOn(i);
            }
        }

        // Send command
        this->_rawCommand.write(cmd);

        // TODO: motion command not yet supported (phbaer)
    }
}

// void Local::errorHook() {}
// void Local::stopHook() {}
// void Local::cleanupHook() {}

