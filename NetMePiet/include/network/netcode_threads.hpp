#pragma once

//===== ===== EXTERN ===== =====
#include <concurrentqueue.h>
#include <SDL_net.h>
#include <set>

//===== ===== INTERN ===== =====
#include "messages/message_base.hpp"

namespace NMP::Network {

	using InQueue = moodycamel::ConcurrentQueue<Messages::Base*>;
	using OutQueue = moodycamel::ConcurrentQueue<std::pair<TCPsocket, Messages::Base*>>;

	void ClientConnection(volatile bool& running, IPaddress ip, InQueue& incomingNetworkMessages, OutQueue& outgoingMessages);

	void ServerAcceptNewConnections(volatile bool& running, IPaddress ip);

	void Relay(volatile bool& running, OutQueue& incomingClientNetworkMessages, std::set<TCPsocket>* startPeers = nullptr);

	void HandleConnection(volatile bool& running, TCPsocket socket, OutQueue& queue);

} // NMP::Network
