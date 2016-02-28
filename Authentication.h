#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H
#include "Packet.h"
#include "Client.h"
#include <mysql.h>

class Authentication
{
	public:
		static void login(Client* c, Packet& p, MYSQL* con);

};

#endif // AUTHENTICATION_H
