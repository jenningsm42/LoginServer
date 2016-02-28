#include "Logger.h"
#include <fstream>
#include <iostream>
#include <ctime>
#include <cstdarg>
#include <thread>

bool locked = false;
void Logger::log(const char* msg, ...)
{
    while(locked) std::this_thread::sleep_for(std::chrono::milliseconds(1));

    locked = true;
    va_list args;
    va_start(args, msg);

    time_t rawtime;
    struct tm * timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    char timestr[64];
    strftime(timestr, 64, "%H:%M:%S", timeinfo);

    char buf1[1024], buf2[1024];
    sprintf(buf1, "[%s] %s", timestr, msg);
    vsprintf(buf2, buf1, args);

    std::cout << buf2 << std::endl;
    std::ofstream o;
    o.open("LoginServer.log", std::ofstream::out | std::ofstream::app);
    o << buf2 << std::endl;
    o.close();
    locked = false;
}
