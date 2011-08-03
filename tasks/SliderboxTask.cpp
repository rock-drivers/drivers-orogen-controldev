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

bool SliderboxTask::startHook()
{
    if (! SliderboxTaskBase::startHook())
        return false;
    
    RTT::extras::FileDescriptorActivity* activity =
        getActivity<RTT::extras::FileDescriptorActivity>();
    if (activity)
    {
	activity->watch(sliderBox->getFileDescriptor());
    }

    return true;
}

void SliderboxTask::updateHook()
{
    SliderboxTaskBase::updateHook();
    
    RawCommand rcmd;

    bool updated = false;
    bool gotValue = false;
    while(this->sliderBox->pollNonBlocking(updated))
	gotValue = true;

    if(!gotValue)
	return;
    
    rcmd.devices = DAI_SliderBox;

    for (int i = 0; i < 7; i++)
    {
	rcmd.sliderValues[i] = this->sliderBox->getValue(i);
    }

    for (int i = 0; i < 4; i++)
    {
	if (this->sliderBox->getButtonOn(i))
	{
	    rcmd.sliderButtons |= (1 << i);
	}
    }

    _raw_command.write(rcmd);
}

void SliderboxTask::stopHook()
{
    RTT::extras::FileDescriptorActivity* activity =
        getActivity<RTT::extras::FileDescriptorActivity>();
    activity->clearAllWatches();

    SliderboxTaskBase::stopHook();
}
