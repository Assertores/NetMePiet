#pragma once

//===== ===== EXTERN ===== =====
#include <SDL_net.h>
#include <set>

//===== ===== INTERN ===== =====
#include "messages/message_base.hpp"
#include "netcode_types.hpp"

namespace NMP::Network {

	void ClientConnection(IPaddress ip, Context* context);

	void ServerAcceptNewConnections(IPaddress ip, Context* context);

	void Relay(Context* context, int count = 0, ...);

	void HandleConnection(volatile std::atomic_bool* running, TCPsocket socket, OutQueue* queue);

} // NMP::Network
