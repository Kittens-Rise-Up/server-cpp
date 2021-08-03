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

	const char* ip = "127.0.0.1";
	int maxClients = 32;
	int maxChannels = 2;
	int incomingBandwidth = 0; // 0 sets to be handled dynamically
	int outgoingBandwidth = 0; // 0 sets to be handled dynamically

	ENetAddress address;
	ENetHost* server;

	address.host = enet_address_set_host(&address, ip);
	address.port = 1234;

	server = enet_host_create(&address, maxClients,	maxChannels, incomingBandwidth, outgoingBandwidth);
	if (server == NULL)
	{
		std::cerr << "An error occurred while trying to create an ENet server host." << std::endl;
		return;
	}

	while (true) 
	{
		ENetEvent event;
		/* Wait up to 1000 milliseconds for an event. */
		while (enet_host_service(server, &event, 15) > 0)
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				printf("A new client connected from %x:%u.\n",
					event.peer->address.host,
					event.peer->address.port);
				/* Store any relevant client information here. */
				event.peer->data = "Client information";
				break;
			case ENET_EVENT_TYPE_RECEIVE:
				printf("A packet of length %u containing %s was received from %s on channel %u.\n",
					event.packet->dataLength,
					event.packet->data,
					event.peer->data,
					event.channelID);
				/* Clean up the packet now that we're done using it. */
				enet_packet_destroy(event.packet);

				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				printf("%s disconnected.\n", event.peer->data);
				/* Reset the peer's client information. */
				event.peer->data = NULL;
			}
		}
	}
	
	enet_host_destroy(server); // Destroy the server
	atexit(enet_deinitialize); // Deinitialize ENet
}