#ifndef SELECTOR_H
#define SELECTOR_H
#include "Client.h"
#include "Job.h"
#include "TCPAcceptor.h"
#include <map>
#include <queue>
#define MAX_CONNECTIONS 2000
#define MAX_EVENTS 128

class Selector
{
	public:
		Selector();
		~Selector();

		bool initialize();
		void monitor(std::queue<Job*>& jobQueue);

	private:
		std::map<int, Client*> clients;
		int epfd;
		TCPAcceptor* acceptor;
		struct epoll_event* events;
};

#endif // SELECTOR_H
