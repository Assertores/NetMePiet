#include "network/netcode_api.hpp"

//===== ===== EXTERN ===== =====
#include <thread>
#include <vector>

//===== ===== INTERN ===== =====
#include "network/messages/message_base.hpp"

#define STANDARD_URL "i.g-s.xyz"
#define STANDARD_PORT 13370

namespace NMP::Network {

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

	void ServerAcceptNewConnections(void) {
		std::vector<std::pair<TCPsocket, std::thread>> clientConnections;

		// enqueued by another thread
		moodycamel::ConcurrentQueue<std::pair<TCPsocket, Messages::Base*>> incomingClientNetworkMessages;

		while(running) {
			TCPsocket new_tcpsock = SDLNet_TCP_Accept(tcpsock);
			if(!new_tcpsock) {
				//!!!!! WARNING Life-lock !!!!!
				printf("SDLNet_TCP_Accept: %s\n", SDLNet_GetError());

				//relay messages
				std::pair<TCPsocket, Messages::Base*> message;
				while(incomingClientNetworkMessages.try_dequeue(message)) {
					if(message.second == nullptr) {
						continue;
					}

					for(auto it = clientConnections.begin(); it != clientConnections.end();){
						if(it->first == message.first) {
							continue;
						}

						if(!SendMessage(message.second, it->first)) {
							it = clientConnections.erase(it);
						} else {
							it++;
						}
					}

					delete(message.second);
				}

				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			} else {
				clientConnections.push_back(std::pair(new_tcpsock, std::thread(HandleConnection, new_tcpsock, &incomingClientNetworkMessages)));
			}
		}

		for(size_t i = 0, size = clientConnections.size(); i < size; i++) {
			clientConnections[i].second.join();
		}
	}

	int InitServer(void) {
		int retVal = Init();
		if(retVal != 0) {
			return retVal;
		}

		tcpsock = SDLNet_TCP_Open(INADDR_ANY);

		running = true;
		listenThread = std::thread(ServerAcceptNewConnections);

		return 0;
	}

	int InitClient(void) {
		int retVal = Init();
		if(retVal != 0) {
			return retVal;
		}

		IPaddress ip;

		//connect
		if(SDLNet_ResolveHost(&ip, STANDARD_URL, STANDARD_PORT) == -1) {
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
