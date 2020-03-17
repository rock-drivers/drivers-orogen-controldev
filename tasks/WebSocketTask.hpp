/* Generated from orogen/lib/orogen/templates/tasks/Task.hpp */

#ifndef CONTROLDEV_WEBSOCKETTASK_TASK_HPP
#define CONTROLDEV_WEBSOCKETTASK_TASK_HPP

#include "controldev/WebSocketTaskBase.hpp"
#include <controldev/RawCommand.hpp>
#include <seasocks/Server.h>
#include <thread>
#include <memory>


namespace controldev{
    struct JoystickHandler;
    /*! \class WebSocketTask
     * \brief The task context provides and requires services. It uses an ExecutionEngine to perform its functions.
     * Essential interfaces are operations, data flow ports and properties. These interfaces have been defined using the oroGen specification.
     * In order to modify the interfaces you should (re)use oroGen and rely on the associated workflow.
     *
     * \details
     * The name of a TaskContext is primarily defined via:
     \verbatim
     deployment 'deployment_name'
         task('custom_task_name','controldev::WebSocketTask')
     end
     \endverbatim
     *  It can be dynamically adapted when the deployment is called with a prefix argument.
     */
    class WebSocketTask : public WebSocketTaskBase
    {
	friend class WebSocketTaskBase;
    friend struct JoystickHandler;

    seasocks::Server *server = nullptr;
    std::thread *thread = nullptr;
    std::shared_ptr<JoystickHandler> handler;
    RawCommand raw_cmd_obj;
    bool controlling;
    bool updateRawCommand(const char *data);

    protected:



    public:
        /** TaskContext constructor for WebSocketTask
         * \param name Name of the task. This name needs to be unique to make it identifiable via nameservices.
         * \param initial_state The initial TaskState of the TaskContext. Default is Stopped state.
         */
        WebSocketTask(std::string const& name = "controldev::WebSocketTask", TaskCore::TaskState initial_state = Stopped);

	    ~WebSocketTask();

        bool configureHook();

        bool startHook();

        void updateHook();

        void errorHook();

        void stopHook();

        void cleanupHook();
    };
}

#endif

