#include "network/netcode_api.hpp"

//===== ===== EXTERN ===== =====
#include <thread>
#include <vector>
#include <set>
#include <atomic>

//===== ===== INTERN ===== =====
#include "network/messages/message_base.hpp"
#include "network/netcode_helper.hpp"
#include "network/netcode_threads.hpp"

#define STANDARD_URL "i.g-s.xyz"
#define STANDARD_PORT 13370

namespace NMP::Network {

	static Context globalContext = Context();

	InQueue& GetInQueue() {
		return globalContext.incomingNetworkMessages;
	}

	int InitServer(uint16_t port/* = 0000*/, Context* context/* = nullptr*/) {
		if(context == nullptr) {
			context = &globalContext;
		}

		int retVal = Init();
		if(retVal != 0) {
			return retVal;
		}

		SetPromiscuousMode(true, context);

		if(port == 0000) {
			port = STANDARD_PORT;
		}

		IPaddress ip;
		ip.host = INADDR_ANY;
		ip.port = port;

		context->running = true;
		//listenThread = std::thread(ServerAcceptNewConnections, running, ip);
		context->listenThread = std::thread([&, ip]() {
			ServerAcceptNewConnections(ip, context);
		});

		return 0;
	}

	int InitClient(std::string hostURL/* = ""*/, uint16_t port/* = 0000*/, Context* context/* = nullptr*/) {
		if(context == nullptr) {
			context = &globalContext;
		}

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

		context->running = true;
		//listenThread = std::thread(ClientConnection, running, ip, incomingNetworkMessages, outMessages);
		context->listenThread = std::thread([&, ip]() {
			ClientConnection(ip, context);
		});

		return 0;
	}

	void SetLobbyID(uint32_t newLobbyID, Context* context/* = nullptr*/) {
		if(context == nullptr) {
			context = &globalContext;
		}

		context->currentLobbyID = newLobbyID;
	}

	void SetPromiscuousMode(bool mode, Context* context/* = nullptr*/) {
		if(context == nullptr) {
			context = &globalContext;
		}

		context->promiscuous = mode;
	}

	void ShutDown(Context* context/* = nullptr*/) {
		if(!CHOOSE_CONTEXT(running)) {
			return;
		}

		CHOOSE_CONTEXT(running) = false;
		CHOOSE_CONTEXT(listenThread).join();

		if(context == nullptr) {
			SDLNet_Quit();
		}
	}

	void SendMessage(Messages::Base* message, Context* context/* = nullptr*/) {
		if(context == nullptr) {
			context = &globalContext;
		}

		if(message == nullptr) {
			return;
		}

		message->_lobbyID = context->currentLobbyID;

		context->outMessages.enqueue(std::pair(nullptr, message));
	}
} // NMP::Network
