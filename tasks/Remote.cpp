#include "Remote.hpp"

using namespace controldev;

Remote::Remote(std::string const& name)
    : RemoteBase(name) {}







/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See Remote.hpp for more detailed
// documentation about them.

// bool Remote::configureHook() { return true; }
// bool Remote::startHook() { return true; }

// void Remote::updateHook() {}

// void Remote::errorHook() {}
// void Remote::stopHook() {}
// void Remote::cleanupHook() {}

