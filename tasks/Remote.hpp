#ifndef CONTROLDEV_REMOTE_TASK_HPP
#define CONTROLDEV_REMOTE_TASK_HPP

#include "controldev/RemoteBase.hpp"

#include <vector>

namespace controldev {
    class Remote : public RemoteBase
    {
	friend class RemoteBase;
    protected:
//        GenericRawCommand grc;
        unsigned int sliders;
        unsigned int buttons;
        unsigned int resolution;
        unsigned int next_sequenznumber;
        unsigned int expected_sequences;
        unsigned int payloadbits;
        unsigned int current_button;
        unsigned int current_slider;
        unsigned int current_sequence_number;
        unsigned char buffered_char;
        unsigned int buffered_charx;
        unsigned int bad_packages;
    	RawCommand rcmd;
    
        virtual bool updateRawCommand(RawCommand& rcmd);
        virtual int getFileDescriptor();

    public:
        Remote(std::string const& name = "controldev::Remote");

        /** This hook is called by Orocos when the state machine transitions
         * from PreOperational to Stopped. If it returns false, then the
         * component will stay in PreOperational. Otherwise, it goes into
         * Stopped.
         *
         * It is meaningful only if the #needs_configuration has been specified
         * in the task context definition with (for example):
         *
         *   task_context "TaskName" do
         *     needs_configuration
         *     ...
         *   end
         */
//         bool configureHook();

        /** This hook is called by Orocos when the state machine transitions
         * from Stopped to Running. If it returns false, then the component will
         * stay in Stopped. Otherwise, it goes into Running and updateHook()
         * will be called.
         */
        // bool startHook();

        /** This hook is called by Orocos when the component is in the Running
         * state, at each activity step. Here, the activity gives the "ticks"
         * when the hook should be called. See README.txt for different
         * triggering options.
         *
         * The warning(), error() and fatal() calls, when called in this hook,
         * allow to get into the associated RunTimeWarning, RunTimeError and
         * FatalError states. 
         *
         * In the first case, updateHook() is still called, and recovered()
         * allows you to go back into the Running state.  In the second case,
         * the errorHook() will be called instead of updateHook() and in the
         * third case the component is stopped and resetError() needs to be
         * called before starting it again.
         *
         */
//         void updateHook();

        /** This hook is called by Orocos when the component is in the
         * RunTimeError state, at each activity step. See the discussion in
         * updateHook() about triggering options.
         *
         * Call recovered() to go back in the Runtime state.
         */
        // void errorHook();

        /** This hook is called by Orocos when the state machine transitions
         * from Running to Stopped after stop() has been called.
         */
        // void stopHook();

        /** This hook is called by Orocos when the state machine transitions
         * from Stopped to PreOperational, requiring the call to configureHook()
         * before calling start() again.
         */
        // void cleanupHook();

        


    };
}

#endif

