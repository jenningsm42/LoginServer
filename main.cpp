#include "Selector.h"
#include "Dispatcher.h"
#include "Logger.h"
#include <mysql.h>

#include <stdio.h>
#include <stdlib.h>
#include "sha1.h"

int main()
{
    if(mysql_library_init(0, NULL, NULL))
    {
        Logger::log("main: Unable to initialize MySQL library");
        return -1;
    }

    Selector* s = new Selector();
    Dispatcher* d = new Dispatcher();
    bool running = true;
    std::queue<Job*> jobQueue;

    if(!s->initialize())
    {
        mysql_library_end();
        return -1;
    }
    d->initialize();

    Logger::log("main: Server started");

    while(running)
    {
        s->monitor(jobQueue);
        d->dispatch(jobQueue);
    }

    delete d;
    delete s;
    return 0;
}
