#include "Local.hpp"

using namespace controldev;

Local::Local(std::string const& name, TaskCore::TaskState initial_state)
    : LocalBase(name, initial_state) {}






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

// bool Local::configureHook() { return true; }
// bool Local::startHook() { return true; }

// void Local::updateHook() {}

// void Local::errorHook() {}
// void Local::stopHook() {}
// void Local::cleanupHook() {}

