#pragma once

#include <atomic>
#include <enet/enet.h>

class Server 
{
public:
	static void WorkerThread();
	static std::atomic<bool> m_StopWorker;
};