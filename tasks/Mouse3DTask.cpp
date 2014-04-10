/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "Mouse3DTask.hpp"
#include <controldev/ConnexionHID.hpp>
#include <rtt/extras/FileDescriptorActivity.hpp>

using namespace controldev;

Mouse3DTask::Mouse3DTask(std::string const& name)
    : Mouse3DTaskBase(name)
{
    interface = new ConnexionHID();
    std::vector<double> v= _axisScale.get();
    v.resize(6);
    for(int i=0;i<6;i++){
        v[i] = interface->axisScalinig((ConnexionHID::Mapping)i); 
    }
    _axisScale.set(v);

}

Mouse3DTask::Mouse3DTask(std::string const& name, RTT::ExecutionEngine* engine)
    : Mouse3DTaskBase(name, engine)
{
    interface = new ConnexionHID();
    std::vector<double> v= _axisScale.get();
    v.resize(6);
    for(int i=0;i<6;i++){
        v[i] = interface->axisScalinig((ConnexionHID::Mapping)i); 
    }
    _axisScale.set(v);

}

Mouse3DTask::~Mouse3DTask()
{
    delete interface;
}



/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See Mouse3DTask.hpp for more detailed
// documentation about them.
bool Mouse3DTask::configureHook()
{
    if (! controldev::GenericTask::configureHook())
        return false;


    if (!interface->init())
    {
        std::cerr << "Warning: Unable to open 3D Mousedevice" << std::endl;
	return false;
    }

    std::vector<double> v= _axisScale.get();
    if(v.size() != 6){
        std::cerr << "Axis scale has wrong dimension" << std::endl;
        return false;
    }
    for(int i=0;i<6;i++){
        interface->axisScalinig((ConnexionHID::Mapping)i) = v[i]; 
    }
    return true;
    
}



bool Mouse3DTask::startHook()
{
    
    if (! controldev::GenericTask::startHook())
        return false;
    
    RTT::extras::FileDescriptorActivity* activity =
        getActivity<RTT::extras::FileDescriptorActivity>();
    if (activity)
    {
	activity->watch(interface->getFileDescriptor());
    }
    return true;
    
}


void Mouse3DTask::updateHook()
{
    
    controldev::GenericTask::updateHook();
   
    RawCommand rcmd;
    rcmd.deviceIdentifier= "3DMouse";
   
    rcmd.axes.elements.resize(6);
   
    connexionValues values;
    connexionValues rawValues;
    interface->getValue(values,rawValues);

    rcmd.axes.elements[0] = values.tx;
    rcmd.axes.elements[1] = values.ty;
    rcmd.axes.elements[2] = values.tz;
    rcmd.axes.elements[3] = values.rx;
    rcmd.axes.elements[4] = values.ry;
    rcmd.axes.elements[5] = values.rz;
    rcmd.buttons.elements.push_back(values.button1);
    rcmd.buttons.elements.push_back(values.button2);
    _raw_command.write(rcmd);
}



void Mouse3DTask::errorHook()
{
    
    controldev::GenericTask::errorHook();
}



void Mouse3DTask::stopHook()
{
    
    controldev::GenericTask::stopHook();
}



void Mouse3DTask::cleanupHook()
{
    
    controldev::GenericTask::cleanupHook();
}

