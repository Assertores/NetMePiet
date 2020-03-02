#include "network/netcode_api.hpp"

//===== ===== EXTERN ===== =====
#include <thread>
#include <vector>
#include <set>

//===== ===== INTERN ===== =====
#include "network/messages/message_base.hpp"
#include "network/netcode_helper.hpp"
#include "network/netcode_threads.hpp"

#define STANDARD_URL "i.g-s.xyz"
#define STANDARD_PORT 13370

namespace NMP::Network {

	InQueue incomingNetworkMessages {};
	OutQueue outMessages;

	static bool running = false;
	bool promiscuous = false;
	uint32_t currentLobbyID = 0;
	std::thread listenThread;

	int InitServer(uint16_t port/* = 0000*/) {
		int retVal = Init();
		if(retVal != 0) {
			return retVal;
		}

		SetPromiscuousMode(true);

		if(port == 0000) {
			port = STANDARD_PORT;
		}

		IPaddress ip;
		ip.host = INADDR_ANY;
		ip.port = port;

		running = true;
		//listenThread = std::thread(ServerAcceptNewConnections, running, ip);
		listenThread = std::thread([ip]() {
			ServerAcceptNewConnections(running, ip);
		});

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

		running = true;
		//listenThread = std::thread(ClientConnection, running, ip, incomingNetworkMessages, outMessages);
		listenThread = std::thread([ip]() {
			ClientConnection(running, ip, promiscuous, currentLobbyID, incomingNetworkMessages, outMessages);
		});

		return 0;
	}

	void SetLobbyID(uint32_t newLobbyID) {
		currentLobbyID = newLobbyID;
	}

	void SetPromiscuousMode(bool mode) {
		promiscuous = mode;
	}

	void ShutDown(void) {
		if(!running) {
			return;
		}

		SDLNet_Quit();

		running = false;
		listenThread.join();
	}

	void SendMessage(Messages::Base* message) {
		if(message == nullptr) {
			return;
		}

		outMessages.enqueue(std::pair(nullptr, message));
	}
} // NMP::Network
