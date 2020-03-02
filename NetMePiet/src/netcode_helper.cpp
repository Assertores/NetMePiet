#include "network/netcode_helper.hpp"

//===== ===== EXTERN ===== =====
#include <SDL.h>
#include <SDL_net.h>
#include <concurrentqueue.h>

//===== ===== INTERN ===== =====

namespace NMP::Network {

	extern moodycamel::ConcurrentQueue<Messages::Base*> incomingNetworkMessages;
	extern bool promiscuous;
	extern uint32_t currentLobbyID;

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

	Messages::Base* WaitForMessageComplete(TCPsocket listenerSocket, uint8_t buffer[], size_t maxSize) {

		//listen
		auto result = SDLNet_TCP_Recv(listenerSocket, buffer, 4);
		if(result <= 0) {
			// An error may have occured, but sometimes you can just ignore it
			return nullptr;
		}

		uint32_t packageSize = ((uint32_t*)buffer)[0];
		if(packageSize > maxSize) {
			//TODO: incomming package bigger than buffer
			return nullptr;
		}

		result = SDLNet_TCP_Recv(listenerSocket, buffer, packageSize);
		if(result <= 0) {
			// An error may have occured, but sometimes you can just ignore it
			return nullptr;
		}

		return Messages::Base::Deserialize(buffer, packageSize);
	}

	bool DoSendMessage(Messages::Base* message, TCPsocket socket) {
		if(message == nullptr) {
			return true;
		}

		if(socket == nullptr) {
			return false;
		}

		uint8_t buffer[1024];
		size_t packageSize = message->Serialize(buffer + 4, 1024);
		((uint32_t*)buffer)[0] = (uint32_t)packageSize;

		auto result = SDLNet_TCP_Send(socket, buffer, (int)packageSize + 4);
		if(result < packageSize) {
			printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
			return false;
		}

		if(promiscuous || message->_lobbyID == currentLobbyID) {
			incomingNetworkMessages.enqueue(message);
		}

		return true;
	}

} // NMP::Network
