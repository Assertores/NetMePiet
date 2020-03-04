#pragma once

#include <concurrentqueue.h>

#include "messages/message_base.hpp"

namespace NMP::Network {

	using InQueue = moodycamel::ConcurrentQueue<Messages::Base*>;
	using OutQueue = moodycamel::ConcurrentQueue<std::pair<TCPsocket, Messages::Base*>>;

	struct Context {
		InQueue incomingNetworkMessages;
		OutQueue outMessages;

		std::atomic_bool running;
		std::atomic_bool promiscuous;
		std::atomic_uint32_t currentLobbyID;
		std::thread listenThread;

		Context() {
			running = false;
			promiscuous = false;
			currentLobbyID = 0;
		}
	};
} // NMP::Network
