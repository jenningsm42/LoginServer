#include "Client.h"

Client::Client(TCPStream* s) : stream(s), username(""), type(0), authenticated(false)
{
}

Client::~Client()
{
    delete stream;
}

void Client::setUsername(char val[24])
{
	username = val;
}

std::string Client::getUsername()
{
    return username;
}

void Client::setType(char val)
{
	type = val;
}

char Client::getType()
{
	return type;
}

TCPStream* Client::getStream()
{
	return stream;
}

void Client::setAuthenticated(bool val)
{
	authenticated = val;
}

bool Client::isAuthenticated()
{
	return authenticated;
}
