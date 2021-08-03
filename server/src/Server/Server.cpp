#include "vkpch.h"
#include "Server.h"

std::atomic<bool> Server::m_StopWorker;

void Server::WorkerThread() 
{
	if (m_StopWorker)
		return;

	if (enet_initialize() != 0)
	{
		std::cerr << "Failed to initialize ENet" << std::endl;
		return;
	}
	
	atexit(enet_deinitialize);
}