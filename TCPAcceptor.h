#ifndef TCPACCEPTOR_H
#define TCPACCEPTOR_H
#include "TCPStream.h"
#include <netinet/in.h>

class TCPAcceptor
{
public:
    TCPAcceptor(int port, const char* address = "");
    ~TCPAcceptor();

    int start();
    TCPStream* accept();

    int getListeningSocketDescriptor();

private:
    int listeningSocketDescriptor;
    int port;
    std::string address;
    bool listening;
};

#endif // TCPACCEPTOR_H
