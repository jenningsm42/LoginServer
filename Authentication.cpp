#include "Authentication.h"
#include "PacketTypes.h"
#include "sha1.h"

void Authentication::login(Client* c, Packet& p, MYSQL* con)
{
	if(c->isAuthenticated())
	{
		Packet pack(E_PT_SEND_ALREADY_AUTHENTICATED);
		c->getStream()->send(pack);
		return;
	}

	char* password = p.popCharArray(24);
	char* username = p.popCharArray(24);

	char query[256];
	snprintf(query, 256, "SELECT * FROM accounts WHERE username='%s';", username);
	mysql_query(con, query);
	MYSQL_RES* res = mysql_store_result(con);
	MYSQL_ROW row;

	if((row = mysql_fetch_row(res)))
	{
		unsigned char* salt = (unsigned char*)row[2],* hash = (unsigned char*)row[3];
		for(int i = 0; i < 16; i++)
			printf("%02x ", salt[i]);
		printf("\n");
		for(int i = 0; i < 20; i++)
			printf("%02x ", hash[i]);
		printf("\n");
		unsigned char newhash[20];
		unsigned char* buffer = new unsigned char[16 + strlen(password)];
		memcpy(buffer, salt, 16);
		memcpy(buffer + 16, password, strlen(password));
		sha1(buffer, 16 + strlen(password), newhash);
		for(int i = 0; i < 20; i++)
			printf("%02x ", newhash[i]);
		printf("\n");
		if(!memcmp(hash, newhash, 20))
		{
			c->setAuthenticated(true);

			Packet pack(E_PT_SEND_LOGIN_SUCCESS);
			c->getStream()->send(pack);

			mysql_free_result(res);

			sprintf(query, "UPDATE accounts SET lastlogin=NOW() WHERE username='%s';", username);
			mysql_query(con, query);

			return;
		}

		Packet pack(E_PT_SEND_INVALID_PASSWORD);
		c->getStream()->send(pack);

		mysql_free_result(res);
		return;
	}
	Packet pack(E_PT_SEND_INVALID_USERNAME);
	c->getStream()->send(pack);

	mysql_free_result(res);
	return;
}
