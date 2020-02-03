/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "WebSocketTask.hpp"

using namespace controldev;

WebSocketTask::WebSocketTask(std::string const& name, TaskCore::TaskState initial_state)
    : WebSocketTaskBase(name, initial_state)
{
}

WebSocketTask::~WebSocketTask()
{
}



/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See WebSocketTask.hpp for more detailed
// documentation about them.

bool WebSocketTask::configureHook()
{
    if (! WebSocketTaskBase::configureHook())
        return false;
    return true;
}
bool WebSocketTask::startHook()
{
    if (! WebSocketTaskBase::startHook())
        return false;
    return true;
}
void WebSocketTask::updateHook()
{
    WebSocketTaskBase::updateHook();
}
void WebSocketTask::errorHook()
{
    WebSocketTaskBase::errorHook();
}
void WebSocketTask::stopHook()
{
    WebSocketTaskBase::stopHook();
}
void WebSocketTask::cleanupHook()
{
    WebSocketTaskBase::cleanupHook();
}
