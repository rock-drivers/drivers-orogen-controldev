#include "GenericTask.hpp"
#include <rtt/extras/FileDescriptorActivity.hpp>

using namespace controldev;

GenericTask::GenericTask(std::string const& name)
    : GenericTaskBase(name) {}

GenericTask::GenericTask(std::string const& name, RTT::ExecutionEngine* engine)
    : GenericTaskBase(name, engine)
{
}

GenericTask::~GenericTask()
{
}

bool GenericTask::configureHook()
{
    axisHandles.clear();
    
    const std::vector<AxisPort> &portDescs(_axisPorts.get());
    for(size_t i = 0 ; i < portDescs.size(); i++)
    {
        AxisPortHandle handle;
        handle.portDesc = portDescs[i];
        handle.port = new RTT::OutputPort<double>();
        
        ports()->addPort(handle.portDesc.portName, *handle.port);
        
        axisHandles.push_back(handle);
    }
    
    axisScales = _axisScale.get();
    
    return TaskContext::configureHook();
};

bool GenericTask::startHook()
{
    RTT::extras::FileDescriptorActivity* activity =
        getActivity<RTT::extras::FileDescriptorActivity>();
    if (activity)
    {
        activity->watch(getFileDescriptor());
        //get trigger a least every 25 ms
        activity->setTimeout(25);
    }

    return TaskContext::startHook();
}

void GenericTask::updateHook()
{
    GenericTaskBase::updateHook();
    
     
    RawCommand rcmd;
    updateRawCommand(rcmd);

    if(axisScales.size() > rcmd.axisValue.size())
    {
        std::cout << "Error, more scale factors than axes defined : Nr axes " << rcmd.axisValue.size() << " size of axis scales " << axisScales.size() << std::endl;
        exception();
    }
    
    RawCommand rscaled = rcmd;
    for(size_t i = 0 ; i < axisScales.size(); i++)
    {
        rscaled.axisValue[i] *= axisScales[i];
    }
    
    _raw_command.write(rscaled);

    for(size_t i = 0 ; i < axisHandles.size(); i++)
    {
        const AxisPortHandle &handle(axisHandles[i]);
        const AxisPort &desc(handle.portDesc);
        if(desc.axisNr < 0 || desc.axisNr > rcmd.axisValue.size())
        {
            std::cout << "Error, there is no axis " << desc.axisNr << " available. Max nr axis is " << rcmd.axisValue.size() << std::endl;
            exception();
        }
        
        const double value = rcmd.axisValue[desc.axisNr] * desc.scaleFactor + desc.offset;
        handle.port->write(value);
    }
};

void GenericTask::stopHook()
{
    RTT::extras::FileDescriptorActivity* activity =
        getActivity<RTT::extras::FileDescriptorActivity>();
        
    if(activity)
        activity->clearAllWatches();
}

