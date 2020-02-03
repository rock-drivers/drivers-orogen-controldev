#ifndef CONTROLDEV_WEBSOCKETTASK_TASK_HPP
#define CONTROLDEV_WEBSOCKETTASK_TASK_HPP

#include <libsoup/soup.h>
#include <json-glib/json-glib.h>
#include <string.h>

namespace controldev {
    class WebSocketTask {

    public:
        WebSocketTask();

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
