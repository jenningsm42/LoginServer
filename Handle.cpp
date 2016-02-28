#include "Handle.h"
#include "PacketTypes.h"
#include "Logger.h"
#include "Authentication.h"
#include <chrono>

Handle::Handle() : t(std::thread(&Handle::run, this)), running(true), con(NULL)
{
}

Handle::~Handle()
{
    mysql_thread_end();
    mysql_close(con);
    running = false;
    if(t.joinable()) t.join();
}

bool Handle::initialize()
{
    if(!(con = mysql_init(NULL)))
    {
        Logger::log("Handle: Unable to initialize MySQL connection (%d)", mysql_error(con));
        return false;
    }

    if(!mysql_real_connect(con, "127.0.0.1", "loginserv", "#QlM!n0y3e2^zX", "TestServer", 0, NULL, 0))
    {
        Logger::log("Handle: Unable to connect to MySQL database (%d)", mysql_error(con));
        return false;
    }

    return true;
}

void Handle::addJob(Job* j)
{
    jobQueue.push(j);
}

int Handle::getJobCount()
{
    return jobQueue.size();
}

void Handle::run()
{
    while(running)
    {
        if(!jobQueue.empty())
        {
            Job* j = jobQueue.front();
            jobQueue.pop();

            Packet p = j->getClient()->getStream()->recv();
            switch(p.getType())
            {
            case E_PT_RECV_LOGIN:
                Authentication::login(j->getClient(), p, con);
                break;
            default:
                Logger::log("Handle: Received invalid packet (type: %d)", p.getType());
                break;
            }
        }
        else std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
