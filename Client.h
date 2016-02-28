#ifndef CLIENT_H
#define CLIENT_H
#include "TCPStream.h"

class Client
{
    public:
        Client(TCPStream* stream);
        ~Client();

        void setUsername(char val[24]);
        std::string getUsername();
        void setType(char val);
        char getType();
        TCPStream* getStream();

        void setAuthenticated(bool val);
        bool isAuthenticated();

    private:
        TCPStream* stream;
        std::string username;
        char type;
        bool authenticated;
};

#endif // CLIENT_H
