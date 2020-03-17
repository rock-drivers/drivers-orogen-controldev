/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "GraupnerMC20Task.hpp"

using namespace controldev;

GraupnerMC20Task::GraupnerMC20Task(std::string const& name)
    : GraupnerMC20TaskBase(name)
{
}

GraupnerMC20Task::GraupnerMC20Task(std::string const& name, RTT::ExecutionEngine* engine)
    : GraupnerMC20TaskBase(name, engine)
{
}

GraupnerMC20Task::~GraupnerMC20Task()
{
}



/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See GraupnerMC20Task.hpp for more detailed
// documentation about them.

bool GraupnerMC20Task::configureHook()
{
    driver.openURI(_io_port.get()); 
    this->setDriver(&driver);

    if (! GraupnerMC20TaskBase::configureHook())
        return false;
    return true;
}
bool GraupnerMC20Task::startHook()
{
    if (! GraupnerMC20TaskBase::startHook())
        return false;
    return true;
}
void GraupnerMC20Task::updateHook()
{
    GraupnerMC20TaskBase::updateHook();
}
void GraupnerMC20Task::errorHook()
{
    GraupnerMC20TaskBase::errorHook();
}
void GraupnerMC20Task::stopHook()
{
    GraupnerMC20TaskBase::stopHook();
}
void GraupnerMC20Task::cleanupHook()
{
    GraupnerMC20TaskBase::cleanupHook();
}
void GraupnerMC20Task::processIO()
{
    controldev::RawCommand rcmd;
    rcmd.time = base::Time::now();
    MC20Output out;
    driver.getReading(&out);
    rcmd.deviceIdentifier= "mc20";

    rcmd.axisValue.resize(6, 0);
    for (unsigned i=0; i < rcmd.axisValue.size(); i++)
    {
        rcmd.axisValue[i] = (out.channel[i+1] - 12000) / 3200.0;

        if (std::abs(rcmd.axisValue[i]) <= 0.01)
            rcmd.axisValue[i] = 0.0;
    }

    rcmd.buttonValue.resize(9, 0);
    for (unsigned i=0; i < rcmd.buttonValue.size(); i++)
    {
        if (out.channel[i+7] > 14000)
        {
            rcmd.buttonValue[i] = 1;
        }
    }
    _raw_command.write(rcmd);
}
