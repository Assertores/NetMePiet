#include "network/netcode_threads.hpp"

//===== ===== EXTERN ===== =====
#include <vector>
#include <map>
#include <stdarg.h>

//===== ===== INTERN ===== =====
#include "network/netcode_helper.hpp"

namespace NMP::Network {

	void ClientConnection(IPaddress ip, Context* context) {
		TCPsocket tcpsock = SDLNet_TCP_Open(&ip);
		if(!tcpsock) {
			printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
			return;
		}

		std::thread relay = std::thread([context, tcpsock]() {
			Relay(context, 1, tcpsock);
		});

		uint8_t buffer[1024];

		while(context->running) {
			Messages::Base* m = WaitForMessageComplete(tcpsock, buffer, 1024);

			if(m != nullptr) {
				if(context->promiscuous || m->_lobbyID == context->currentLobbyID) {
					context->incomingNetworkMessages.enqueue(m);
				}
			} else {
				return;
			}
		}

		relay.join();
	}

	void ServerAcceptNewConnections(IPaddress ip, Context* context) {
		TCPsocket tcpsock = SDLNet_TCP_Open(&ip);
		if(!tcpsock) {
			printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
			return;
		}

		std::vector<std::pair<TCPsocket, std::thread>> clientConnections;

		std::thread relay = std::thread([context]() {
			Relay(context);
		});

		while(context->running) {
			TCPsocket new_tcpsock = SDLNet_TCP_Accept(tcpsock);
			if(!new_tcpsock) {
				//printf("SDLNet_TCP_Accept: %s\n", SDLNet_GetError());

				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			} else {
				printf("ne connection");
				clientConnections.push_back(std::pair(
					new_tcpsock,
					std::thread([&]() {
						HandleConnection(&context->running, new_tcpsock, &context->outMessages);
					})
				));
			}
		}

		relay.join();

		for(size_t i = 0, size = clientConnections.size(); i < size; i++) {
			clientConnections[i].second.join();
		}
	}

	void HandleConnection(volatile std::atomic_bool* running, TCPsocket socket, OutQueue* queue) {
		uint8_t buffer[1024];

		while(*running) {
			Messages::Base* m = WaitForMessageComplete(socket, buffer, 1024);

			if(m != nullptr) {
				queue->enqueue(std::pair(socket, m));
			} else {
				return;
			}
		}
	}

	void Relay(Context* context, int count/* = 0*/, ...) {
		std::map<TCPsocket, uint32_t> clients;

		va_list valist;
		va_start(valist, count);

		for(int i = 0; i < count; i++) {
			clients[va_arg(valist, TCPsocket)] = 0;
		}

		va_end(valist);

		while(context->running) {
			std::pair<TCPsocket, Messages::Base*> message;
			while(context->outMessages.try_dequeue(message)) {
				if(message.second == nullptr) {
					continue;
				}

				for(auto it = clients.begin(); it != clients.end();) {
					if((*it).first == message.first) {
						continue;
					}
					if(!((*it).second == 0 || (*it).second == message.second->_lobbyID)) {
						continue;
					}

					if(!DoSendMessage(message.second, (*it).first, context)) {
						it = clients.erase(it);
					} else {
						it++;
					}
				}
				delete(message.second);

				if(message.first == nullptr) {
					continue;
				}

				clients[message.first] = message.second->_lobbyID;
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

} // NMP::Network
