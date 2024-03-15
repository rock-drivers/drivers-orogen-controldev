/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "RawCommandAggregator.hpp"

using namespace controldev;

RawCommandAggregator::RawCommandAggregator(std::string const& name,
    TaskCore::TaskState initial_state)
    : RawCommandAggregatorBase(name, initial_state)
{
}

RawCommandAggregator::~RawCommandAggregator()
{
}

/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See RawCommandAggregator.hpp for more detailed
// documentation about them.

bool RawCommandAggregator::configureHook()
{
    if (!RawCommandAggregatorBase::configureHook())
        return false;
    return true;
}
bool RawCommandAggregator::startHook()
{
    if (!RawCommandAggregatorBase::startHook())
        return false;
    return true;
}
void RawCommandAggregator::updateHook()
{
    RawCommandAggregatorBase::updateHook();

    controldev::RawCommand command;
    while (_command_in.read(command) == RTT::NewData) {
        if (command.time <= m_most_recent_command_time) {
            continue;
        }

        _command_out.write(command);

        m_most_recent_command_time = command.time;
    }
}
void RawCommandAggregator::errorHook()
{
    RawCommandAggregatorBase::errorHook();
}
void RawCommandAggregator::stopHook()
{
    RawCommandAggregatorBase::stopHook();
}
void RawCommandAggregator::cleanupHook()
{
    RawCommandAggregatorBase::cleanupHook();
}
