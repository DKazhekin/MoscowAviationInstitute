#include <iostream>
#include <unistd.h>
#include <string>
#include <sstream>
#include <exception>
#include <signal.h>
#include "zmq.hpp"
#include "timer.h"

const int WAIT_TIME = 1000;
const int PORT_BASE  = 5050;

bool send_message(zmq::socket_t &socket, const std::string &message_string)
{
    zmq::message_t message(message_string.size());
    memcpy(message.data(), message_string.c_str(), message_string.size());
    return socket.send(message);
}

std::string recieve_message(zmq::socket_t &socket)
{
    zmq::message_t message;
    bool ok = false;
    try
    {
        ok = socket.recv(&message);
    }
    catch (...)
    {
        ok = false;
    }
    std::string recieved_message(static_cast<char*>(message.data()), message.size());
    if (recieved_message.empty() || !ok)
    {
        return "Error: Node is not available";
    }
    return recieved_message;
}

void create_node(int id, int port)
{
    char* arg0 = strdup("./client");
    char* arg1 = strdup((std::to_string(id)).c_str());
    char* arg2 = strdup((std::to_string(port)).c_str());
    char* args[] = {arg0, arg1, arg2, nullptr};
    execv("./client", args);
}

std::string get_port_name(const int port)
{
    return "tcp://127.0.0.1:" + std::to_string(port);
}

void rl_create(zmq::socket_t& parent_socket, zmq::socket_t& socket, int& create_id, int& id, int& pid)
{
    if (pid == -1)
    {
        send_message(parent_socket, "Error: Cannot fork");
        pid = 0;
    }
    else if (pid == 0)
    {
        create_node(create_id,PORT_BASE + create_id);
    }
    else
    {
        id = create_id;
        send_message(socket, "pid");
        send_message(parent_socket, recieve_message(socket));
    }
}

void rl_kill(zmq::socket_t& parent_socket, zmq::socket_t& socket,  int& delete_id, int& id, int& pid, std::string& request_string)
{
    if (id == 0)
    {
        send_message(parent_socket, "Error: Not found");
    }
    else if (id == delete_id)
    {
        send_message(socket, "kill_children");
        recieve_message(socket);
        kill(pid,SIGTERM);
        kill(pid,SIGKILL);
        id = 0;
        pid = 0;
        send_message(parent_socket, "Ok");
    }
    else
    {
        send_message(socket, request_string);
        send_message(parent_socket, recieve_message(socket));
    }
}

void rl_exec(zmq::socket_t& parent_socket, zmq::socket_t& socket,  int& id, int& pid, std::string& request_string)
{
    if (pid == 0)
    {
        std::string recieve_message = "Error:" + std::to_string(id);
        recieve_message += ": Not found";
        send_message(parent_socket, recieve_message);
    }
    else
    {
        send_message(socket, request_string);
        send_message(parent_socket, recieve_message(socket));
    }
}

void exec(std::istringstream& command_stream, zmq::socket_t& parent_socket, zmq::socket_t& left_socket,
          zmq::socket_t& right_socket, int& left_pid, int& right_pid, int& id, std::string& request_string, Timer* timer)
{
    std::string subcommand;
    int exec_id;
    command_stream >> exec_id;
    if (exec_id == id)
    {
        command_stream >> subcommand;
        std::string recieve_message = "";
        if (subcommand == "start")
        {
            timer->start_timer();
            recieve_message = "Ok:" + std::to_string(id);
            send_message(parent_socket, recieve_message);
        }
        else if (subcommand == "stop")
        {
            timer->stop_timer();
            recieve_message = "Ok:" + std::to_string(id);
            send_message(parent_socket, recieve_message);
        }
        else if (subcommand == "time")
        {
            recieve_message = "Ok:" + std::to_string(id) + ": ";
            recieve_message += std::to_string(timer->get_time());
            send_message(parent_socket, recieve_message);
        }
    }
