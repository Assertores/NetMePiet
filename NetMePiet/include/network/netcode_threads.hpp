#pragma once

//===== ===== EXTERN ===== =====
#include <concurrentqueue.h>
#include <SDL_net.h>
#include <set>
#include <atomic>

//===== ===== INTERN ===== =====
#include "messages/message_base.hpp"

namespace NMP::Network {

	using InQueue = moodycamel::ConcurrentQueue<Messages::Base*>;
	using OutQueue = moodycamel::ConcurrentQueue<std::pair<TCPsocket, Messages::Base*>>;

	void ClientConnection(volatile std::atomic_bool& running, IPaddress ip, volatile std::atomic_bool& promiscuous, volatile std::atomic_int32_t& lobbyID, InQueue& incomingNetworkMessages, OutQueue& outgoingMessages);

	void ServerAcceptNewConnections(volatile std::atomic_bool& running, IPaddress ip);

	void Relay(volatile std::atomic_bool& running, OutQueue& incomingClientNetworkMessages, int count = 0, ...);

	void HandleConnection(volatile std::atomic_bool& running, TCPsocket socket, OutQueue& queue);

} // NMP::Network
