#pragma once

//===== ===== EXTERN ===== =====
#include <cstdint>
#include <SDL_net.h>
#include <atomic>

//===== ===== INTERN ===== =====
#include "netcode_types.hpp"

#define CHOOSE_CONTEXT(x) ((context == nullptr ? globalContext.x : context->x))

namespace NMP::Network {

	int Init(void);

	Messages::Base* WaitForMessageComplete(TCPsocket listenerSocket, uint8_t buffer[], size_t maxSize);

	bool DoSendMessage(Messages::Base* message, TCPsocket socket = nullptr, Context* context = nullptr);
} // NMP::Network
