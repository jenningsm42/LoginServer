#include "TCPAcceptor.h"
#include "Logger.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <cstdio>
#include <cstring>

TCPAcceptor::TCPAcceptor(int port, const char* address)
    : listeningSocketDescriptor(0), port(port), address(address), listening(false)
{
}

TCPAcceptor::~TCPAcceptor()
{
    if(listeningSocketDescriptor > 0)
    {
        if(close(listeningSocketDescriptor))
            Logger::log("TCPAcceptor: Failed to close socket descriptor (%d)", errno);
    }
}

int TCPAcceptor::start()
{
    if(listening == true) return 0;

    if((listeningSocketDescriptor = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        Logger::log("TCPAcceptor: Failed to create listening socket descriptor (%d)", errno);
        return -1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = PF_INET;
    addr.sin_port = htons(port);
    if(address.size() > 0)
    {
        if(inet_pton(PF_INET, address.c_str(), &(addr.sin_addr)))
        {
            Logger::log("TCPAcceptor: inet_pton failed (%d)", errno);
            return -1;
        }
    }
    else addr.sin_addr.s_addr = INADDR_ANY;

    int optval = 1;
    if(setsockopt(listeningSocketDescriptor, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval))
    {
        Logger::log("TCPAcceptor: setsockopt failed (%d)", errno);
        return -1;
    }

    if(bind(listeningSocketDescriptor, (struct sockaddr*)&addr, sizeof(addr)))
    {
        Logger::log("TCPAcceptor: Failed to bind port (%d)", errno);
        return -1;
    }

    if(listen(listeningSocketDescriptor, 10))
    {
        Logger::log("TCPAcceptor: Failed to start listening (%d)", errno);
        return -1;
    }

    listening = true;
    return 0;
}

TCPStream* TCPAcceptor::accept()
{
    if(!listening) return NULL;

    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    memset(&addr, 0, sizeof(addr));
    int sd = ::accept(listeningSocketDescriptor, (struct sockaddr*)&addr, &len);
    if(sd == -1)
    {
        Logger::log("TCPAcceptor: Failed to accept client (%d)", errno);
        return NULL;
    }
    return new TCPStream(sd, &addr);
}

int TCPAcceptor::getListeningSocketDescriptor()
{
    return listeningSocketDescriptor;
}
