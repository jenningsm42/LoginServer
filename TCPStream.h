#ifndef TCPSTREAM_H
#define TCPSTREAM_H
#include "Packet.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <string>

class TCPStream
{
public:
    ~TCPStream();

    void send(Packet& p);
    Packet recv();

    bool checkDisconnect();

    std::string getPeerIP();
    int getPeerPort();
    int getSocketDescriptor();

    friend class TCPAcceptor;

private:
    TCPStream(int socketDescriptor, struct sockaddr_in* address);

    int socketDescriptor;
    std::string peerIP;
    int peerPort;
};

#endif // TCPSTREAM_H
