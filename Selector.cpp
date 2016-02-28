#include "Selector.h"
#include "Logger.h"
#include "Ports.h"
#include <sys/epoll.h>
#include <errno.h>
#include <thread>
#include <chrono>

Selector::Selector() : epfd(-1)
{
}

Selector::~Selector()
{
	std::map<int, Client*>::iterator it;
	for(it = clients.begin(); it != clients.end(); ++it)
		if(it->second) delete it->second;
	clients.clear();
}

bool Selector::initialize()
{
	acceptor = new TCPAcceptor(LOGINSERVER_PORT);
	if(acceptor->start()) return false;

	if((epfd = epoll_create(MAX_CONNECTIONS)) == -1)
	{
		Logger::log("Selector: Unable to create epoll context (%d)", errno);
		return false;
	}

	struct epoll_event event;
	event.data.fd = acceptor->getListeningSocketDescriptor();
	event.events = EPOLLIN | EPOLLET;
	if(epoll_ctl(epfd, EPOLL_CTL_ADD, acceptor->getListeningSocketDescriptor(), &event))
	{
		Logger::log("Selector: Unable to add listening socket descriptor (%d)", errno);
		return false;
	}

	events = (struct epoll_event*)calloc(MAX_EVENTS, sizeof(struct epoll_event));
	if(!events)
	{
		Logger::log("Selector: Unable to allocate memory to epoll events");
		return false;
	}

	return true;
}

void Selector::monitor(std::queue<Job*>& jobQueue)
{
	int eventsCount = epoll_wait(epfd, events, MAX_EVENTS, -1);
	if(eventsCount < 0)
	{
		Logger::log("Selector: epoll_wait failed, no events");
		free(events);
		return;
	}

	for(int i = 0; i < eventsCount; i++)
	{
		if(events[i].events & EPOLLHUP || events[i].events & EPOLLERR
			|| !(events[i].events & EPOLLIN))
		{
			Client* c = clients.find(events[i].data.fd)->second;
			if(c->getStream()->checkDisconnect())
			{
				Logger::log("Client %d disconnected", events[i].data.fd);
				delete c;
				clients.erase(events[i].data.fd);
				continue;
			}
			continue;
		}

		if(events[i].data.fd == acceptor->getListeningSocketDescriptor())
		{
			TCPStream* stream = acceptor->accept();
			if(stream)
			{
				struct epoll_event event;
				event.data.fd = stream->getSocketDescriptor();
				event.events = EPOLLIN | EPOLLET;
				if(epoll_ctl(epfd, EPOLL_CTL_ADD, stream->getSocketDescriptor(), &event))
				{
					Logger::log("Selector: Failed to add new client to epoll (%d)", errno);
					continue;
				}

				clients.insert(std::pair<int, Client*>(stream->getSocketDescriptor(), new Client(stream)));

				Logger::log("Client %d connected", stream->getSocketDescriptor());
			}
			continue;
		}

		Client* c = clients.find(events[i].data.fd)->second;
		if(c->getStream()->checkDisconnect())
		{
			Logger::log("Client %d disconnected", events[i].data.fd);
			delete c;
			clients.erase(events[i].data.fd);
			continue;
		}

		jobQueue.push(new Job(c));
	}
}
