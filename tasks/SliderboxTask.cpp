/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "SliderboxTask.hpp"
#include <rtt/extras/FileDescriptorActivity.hpp>
#include <controldev/SliderBox.hpp>

using namespace controldev;

SliderboxTask::SliderboxTask(std::string const& name)
    : SliderboxTaskBase(name), sliderBox(new controldev::SliderBox())
{
}

SliderboxTask::SliderboxTask(std::string const& name, RTT::ExecutionEngine* engine)
    : SliderboxTaskBase(name, engine), sliderBox(new controldev::SliderBox())
{
}

SliderboxTask::~SliderboxTask()
{
    delete sliderBox;
}

int SliderboxTask::getFileDescriptor()
{
    return sliderBox->getFileDescriptor();
}

/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See SliderboxTask.hpp for more detailed
// documentation about them.

bool SliderboxTask::configureHook()
{
    if (! SliderboxTaskBase::configureHook())
        return false;
    
    if (!sliderBox->init())
    {
        std::cerr << "Warning: Unable to open SliderBox device, "
             << std::endl;

	return false;
    }

    sliderBox->connectBox();
    
    return true;
}

bool SliderboxTask::updateRawCommand(RawCommand& rcmd)
{

    bool updated = false;
    bool gotValue = false;
    while(this->sliderBox->pollNonBlocking(updated))
        gotValue = true;

    if(!gotValue)
        return false;
    
    rcmd.deviceIdentifier = "Sliderbox";

    rcmd.axisValue.resize(6);
    
    for (int i = 0; i < 7; i++)
    {
        rcmd.axisValue[i] = this->sliderBox->getValue(i);
    }

    for (int i = 0; i < 4; i++)
    {
        rcmd.buttonValue.push_back(this->sliderBox->getButtonOn(i));
    }

    return true;
}
