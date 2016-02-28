#include "TCPStream.h"
#include "PacketTypes.h"
#include "Logger.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

TCPStream::TCPStream(int socketDescriptor, struct sockaddr_in* address) : socketDescriptor(socketDescriptor)
{
    char ip[32];
    inet_ntop(PF_INET, (struct in_addr*)&(address->sin_addr.s_addr), ip, sizeof(ip)-1);
    peerIP = ip;
    peerPort = ntohs(address->sin_port);
}

TCPStream::~TCPStream()
{
    if(close(socketDescriptor) == -1)
        Logger::log("TCPStream: Failed to close socket descriptor (%d)", errno);
}

void TCPStream::send(Packet& p)
{
    char* buffer = p.getBuffer();
    size_t sent = ::send(socketDescriptor, buffer, p.length(), 0);
    while(sent < p.length())
        sent += ::send(socketDescriptor, buffer + sent, p.length() - sent, 0);
    delete buffer;
}

Packet TCPStream::recv()
{
    short len = 0;
    size_t rlen = -1;
    if((rlen = ::recv(socketDescriptor, &len, sizeof(short), 0)) > 0)
    {
        if((len = ntohs(len)) < 0 || len > MAX_LEN)
        {
            Logger::log("TCPStream: Corrupt data received (len is out of bounds)");
            return Packet();
        }

        char* buffer = new char[len + sizeof(short)];
        size_t recvd = ::recv(socketDescriptor, buffer, len + sizeof(short), 0);
        while(recvd < len + sizeof(short))
            recvd += ::recv(socketDescriptor, buffer + recvd, len + sizeof(short) - recvd, 0);
        Packet p(buffer, len);
        delete buffer;
        return p;
    }
    return Packet();
}

bool TCPStream::checkDisconnect()
{
    char nil = 0;
    return ::recv(socketDescriptor, &nil, sizeof(char), MSG_PEEK) == 0;
}

std::string TCPStream::getPeerIP()
{
    return peerIP;
}

int TCPStream::getPeerPort()
{
    return peerPort;
}

int TCPStream::getSocketDescriptor()
{
    return socketDescriptor;
}
