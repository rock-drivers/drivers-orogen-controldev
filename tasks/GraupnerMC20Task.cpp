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
    driver.getReading(&out);
    rcmd.deviceIdentifier= "mc20";
    std::vector<double> axis;
    axis.push_back((out.channel[1]-12000)/3200.0);
    axis.push_back((out.channel[2]-12000)/3200.0);
    axis.push_back((out.channel[3]-12000)/3200.0);
    axis.push_back((out.channel[4]-12000)/3200.0);

    std::vector<double> axis2;
    axis2.push_back((out.channel[5]-12000)/3200.0);
    axis2.push_back((out.channel[6]-12000)/3200.0);

    for(unsigned i = 0; i < axis.size(); i++)
    {
        if (std::abs(axis[i]) <= 0.01)
            axis[i] = 0.0;
    }
    for(unsigned i = 0; i < axis2.size(); i++)
    {
        if (std::abs(axis2[i]) <= 0.01)
            axis2[i] = 0.0;
    }

    rcmd.axisValue.push_back(axis);
    rcmd.axisValue.push_back(axis2);
    rcmd.buttonValue.resize(9, 0);
    for (int i=0; i < 9; i++)
    {
        if (out.channel[i+7] > 14000)
        {
            rcmd.buttonValue[i] = 1;
        }
    }
    _raw_command.write(rcmd);

    //TODO Buttons and scaling

}
