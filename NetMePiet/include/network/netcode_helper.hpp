#pragma once

//===== ===== EXTERN ===== =====
#include <cstdint>
#include <SDL_net.h>

//===== ===== INTERN ===== =====
#include "messages/message_base.hpp"

namespace NMP::Network {

	int Init(void);

	Messages::Base* WaitForMessageComplete(TCPsocket listenerSocket, uint8_t buffer[], size_t maxSize);

	bool DoSendMessage(Messages::Base* message, TCPsocket socket = nullptr);
} // NMP::Network
