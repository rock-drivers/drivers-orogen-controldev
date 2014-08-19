#include "GenericTask.hpp"

using namespace controldev;

GenericTask::GenericTask(std::string const& name, TaskCore::TaskState initial_state)
    : GenericTaskBase(name, initial_state) {}

GenericTask::GenericTask(std::string const& name, RTT::ExecutionEngine* engine, TaskCore::TaskState initial_state)
    : GenericTaskBase(name, engine, initial_state)
{
}

GenericTask::~GenericTask()
{
}



/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See GenericTask.hpp for more detailed
// documentation about them.

// bool GenericTask::configureHook() { return true; }
// bool GenericTask::startHook() { return true; }

// void GenericTask::updateHook() {}

// void GenericTask::errorHook() {}
// void GenericTask::stopHook() {}
// void GenericTask::cleanupHook() {}

