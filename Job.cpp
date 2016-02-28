#include "Job.h"

Job::Job(Client* client) : c(client)
{
}

Job::~Job()
{
}

Client* Job::getClient()
{
	return c;
}
