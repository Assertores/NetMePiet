#include "network/netcode_api.hpp"

//===== ===== EXTERN ===== =====
#include <thread>
#include <vector>
#include <set>

//===== ===== INTERN ===== =====
#include "network/messages/message_base.hpp"

#define STANDARD_URL "i.g-s.xyz"
#define STANDARD_PORT 13370

namespace NMP::Network {

	moodycamel::ConcurrentQueue<Messages::Base*> incomingNetworkMessages {};

	bool running = false;
	std::thread listenThread;
	TCPsocket tcpsock;

	int Init() {
		if(SDL_Init(0) == -1) {
			printf("SDL_Init: %s\n", SDL_GetError());
			return -1;
		}
		if(SDLNet_Init() == -1) {
			printf("SDLNet_Init: %s\n", SDLNet_GetError());
			return -2;
		}

		return 0;
	}

	Messages::Base* WaitForMessageComplete(uint8_t buffer[], size_t maxSize) {

		//listen
		auto result = SDLNet_TCP_Recv(tcpsock, buffer, 4);
		if(result <= 0) {
			// An error may have occured, but sometimes you can just ignore it
			return nullptr;
		}

		uint32_t packageSize = ((uint32_t*)buffer)[0];
		if(packageSize > maxSize) {
			//TODO: incomming package bigger than buffer
			return nullptr;
		}

		result = SDLNet_TCP_Recv(tcpsock, buffer, packageSize);
		if(result <= 0) {
			// An error may have occured, but sometimes you can just ignore it
			return nullptr;
		}

		return Messages::Base::Deserialize(buffer, packageSize);
	}

	void ClientConnection(void) {
		uint8_t buffer[1024];

		while(running) {
			Messages::Base* m = WaitForMessageComplete(buffer, 1024);

			if(m != nullptr) {
				incomingNetworkMessages.enqueue(m);
			} else {
				return;
			}
		}
	}

	void HandleConnection(TCPsocket socket, moodycamel::ConcurrentQueue<std::pair<TCPsocket, Messages::Base*>> *queue) {
		uint8_t buffer[1024];

		while(running) {
			Messages::Base* m = WaitForMessageComplete(buffer, 1024);

			if(m != nullptr) {
				queue->enqueue(std::pair(socket, m));
			} else {
				return;
			}
		}
	}

	//clients have to send at least one package befor they can listen. so everyone who listens has to report himself to the network
	void Relay(moodycamel::ConcurrentQueue<std::pair<TCPsocket, Messages::Base*>>* incomingClientNetworkMessages) {
		std::set<TCPsocket> clients;

		while(running) {
			std::pair<TCPsocket, Messages::Base*> message;
			while(incomingClientNetworkMessages->try_dequeue(message)) {
				if(message.second == nullptr) {
					continue;
				}

				for(auto it = clients.begin(); it != clients.end();) {
					if(*it == message.first) {
						continue;
					}

					if(!SendMessage(message.second, *it)) {
						it = clients.erase(it);
					} else {
						it++;
					}
				}
				delete(message.second);

				if(message.first == nullptr) {
					continue;
				}

				clients.insert(message.first);
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

	void ServerAcceptNewConnections(void) {
		std::vector<std::pair<TCPsocket, std::thread>> clientConnections;

		// enqueued by another thread
		moodycamel::ConcurrentQueue<std::pair<TCPsocket, Messages::Base*>> incomingClientNetworkMessages;

		std::thread relay = std::thread(Relay, &incomingClientNetworkMessages);

		while(running) {
			TCPsocket new_tcpsock = SDLNet_TCP_Accept(tcpsock);
			if(!new_tcpsock) {
				//printf("SDLNet_TCP_Accept: %s\n", SDLNet_GetError());

				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			} else {
				printf("ne connection");
				clientConnections.push_back(std::pair(new_tcpsock, std::thread(HandleConnection, new_tcpsock, &incomingClientNetworkMessages)));
			}
		}

		relay.join();

		for(size_t i = 0, size = clientConnections.size(); i < size; i++) {
			clientConnections[i].second.join();
		}
	}

	int InitServer(uint16_t port/* = 0000*/) {
		int retVal = Init();
		if(retVal != 0) {
			return retVal;
		}

		if(port == 0000) {
			port = STANDARD_PORT;
		}

		IPaddress ip;
		ip.host = INADDR_ANY;
		ip.port = port;

		tcpsock = SDLNet_TCP_Open(&ip);

		running = true;
		listenThread = std::thread(ServerAcceptNewConnections);

		return 0;
	}

	int InitClient(std::string hostURL/* = ""*/, uint16_t port/* = 0000*/) {
		int retVal = Init();
		if(retVal != 0) {
			return retVal;
		}

		if(hostURL == "") {
			hostURL = STANDARD_URL;
		}
		if(port == 0000) {
			port = STANDARD_PORT;
		}

		IPaddress ip;

		//connect
		if(SDLNet_ResolveHost(&ip, hostURL.c_str(), port) == -1) {
			printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
			return 1;
		}

		tcpsock = SDLNet_TCP_Open(&ip);
		if(!tcpsock) {
			printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
			return 2;
		}

		running = true;
		listenThread = std::thread(ClientConnection);

		return 0;
	}

	void ShutDown(void) {
		if(!running) {
			return;
		}

		SDLNet_Quit();

		running = false;
		listenThread.join();
	}

	bool SendMessage(Messages::Base* message, TCPsocket socket/* = nullptr*/) {
		if(message == nullptr) {
			return true;
		}

		if(socket == nullptr) {
			socket = tcpsock;
		}

		uint8_t buffer[1024];
		size_t packageSize = message->Serialize(buffer + 4, 1024);
		((uint32_t*)buffer)[0] = (uint32_t)packageSize;

		auto result = SDLNet_TCP_Send(socket, buffer, (int)packageSize + 4);
		if(result < packageSize) {
			printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
			return false;
		}

		incomingNetworkMessages.enqueue(message);

		return true;
	}
} // NMP::Network
