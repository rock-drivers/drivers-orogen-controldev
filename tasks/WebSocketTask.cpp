/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "WebSocketTask.hpp"

#include <seasocks/PrintfLogger.h>
#include <seasocks/WebSocket.h>
#include <seasocks/Server.h>

#include <json/json.h>

#include <string.h>
#include <iostream>

using namespace controldev;
using namespace seasocks;

struct controldev::JoystickHandler : WebSocket::Handler {
    WebSocket *connection = nullptr;
    WebSocketTask *task = nullptr;
    void onConnect(WebSocket *socket) override{
        if (connection != nullptr){
            connection->close();
        }
        Json::Value msg = true;
        socket->send(msg.asString());
        connection = socket;
    }
    void onData(WebSocket *socket, const char *data) override{
        Json::Value msg;
        msg = task->updateRawCommand(data);
        connection->send(msg.asString());
        if (msg.asBool() && task->controlling)
            task->_raw_command.write(task->raw_cmd_obj);
    }
    void onDisconnect(WebSocket *socket) override{
        connection = nullptr;
    }
};

bool WebSocketTask::updateRawCommand(const char *data){
    Json::Value jdata;
    Json::CharReaderBuilder rbuilder;
    std::unique_ptr<Json::CharReader> const reader(rbuilder.newCharReader());
    std::string errs;

    if (!reader->parse(data, data+std::strlen(data), &jdata, &errs)){
        std::cout << errs << std::endl;
        return false;
    }

    try{
        raw_cmd_obj.axisValue.clear();
        raw_cmd_obj.buttonValue.clear();

        raw_cmd_obj.axisValue[0] = jdata["axes"][0].asDouble();
        raw_cmd_obj.axisValue[1] = jdata["axes"][1].asDouble();
        raw_cmd_obj.axisValue[2] = jdata["buttons"][6].asDouble() * 2 - 1;
        raw_cmd_obj.axisValue[3] = jdata["axes"][2].asDouble();
        raw_cmd_obj.axisValue[4] = jdata["axes"][3].asDouble();
        raw_cmd_obj.axisValue[5] = jdata["buttons"][7].asDouble() * 2 - 1;
        raw_cmd_obj.axisValue[6] = jdata["buttons"][15].asDouble() - jdata["buttons"][14].asDouble();
        raw_cmd_obj.axisValue[7] = jdata["buttons"][13].asDouble() - jdata["buttons"][12].asDouble();

        raw_cmd_obj.buttonValue[0] = jdata["buttons"][0].asUInt();
        raw_cmd_obj.buttonValue[1] = jdata["buttons"][1].asUInt();
        raw_cmd_obj.buttonValue[2] = jdata["buttons"][2].asUInt();
        raw_cmd_obj.buttonValue[3] = jdata["buttons"][3].asUInt();
        raw_cmd_obj.buttonValue[4] = jdata["buttons"][4].asUInt();
        raw_cmd_obj.buttonValue[5] = jdata["buttons"][5].asUInt();
        raw_cmd_obj.buttonValue[6] = jdata["buttons"][8].asUInt();
        raw_cmd_obj.buttonValue[7] = jdata["buttons"][9].asUInt();

    } catch (std::exception e){
        std::cout << e.what() << std::endl;
        return false;
    }
    return true;
}

WebSocketTask::WebSocketTask(std::string const& name, TaskCore::TaskState initial_state)
    : WebSocketTaskBase(name, initial_state)
{
    for (int i=1; i<=8; ++i){
        raw_cmd_obj.buttonValue.push_back(0);
        raw_cmd_obj.axisValue.push_back(0.0);
    }
}

WebSocketTask::~WebSocketTask()
{
}

bool WebSocketTask::configureHook()
{
    if (! WebSocketTaskBase::configureHook())
        return false;

    auto logger = std::make_shared<PrintfLogger>(Logger::Level::Debug);
    server = new Server (logger);
    handler = std::make_shared<JoystickHandler>();
    handler->task = this;

    std::cout << "configured" << std::endl;
    return true;
}
bool WebSocketTask::startHook()
{
    if (! WebSocketTaskBase::startHook())
        return false;

    server->addWebSocketHandler("/ws", handler, true);
    if (!server->startListening(_port.get())) {
        return false;
    }

    thread = new std::thread([this]{ this->server->loop(); });

    return true;
}
void WebSocketTask::updateHook()
{
    WebSocketTaskBase::updateHook();
}
void WebSocketTask::errorHook()
{
    WebSocketTaskBase::errorHook();
    server->terminate();
    thread->join();
}
void WebSocketTask::stopHook()
{
    WebSocketTaskBase::stopHook();
    server->terminate();
    thread->join();
}
void WebSocketTask::cleanupHook()
{
    WebSocketTaskBase::cleanupHook();
    delete server;
    delete thread;
}