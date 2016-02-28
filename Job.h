#ifndef JOB_H
#define JOB_H
#include "Client.h"

class Job
{
	public:
		Job(Client* client);
		~Job();

		Client* getClient();

	private:
		Client* c;
};

#endif // JOB_H
