#include "network/messages/message_chat.hpp"

//===== ===== EXTERN ===== =====
#include <iostream>

//===== ===== INTERN ===== =====

namespace NMP::Network::Messages {

	size_t Chat::DataSerialize(uint8_t buffer[], const size_t maxSize) const {
		if(buffer == nullptr) {
			return 0;
		}

		size_t payloadSize = sizeof(uint32_t) + message.size() + 1; //null character
		if(maxSize < payloadSize) {
			return 0;
		}

		*((uint32_t*)buffer) = _clientID;
		buffer += sizeof(uint32_t);

		memcpy((void*)buffer, message.data(), message.size() + 1);
		buffer += message.size() + 1;

		return payloadSize;
	}

	void Chat::DataDeserialize(const uint8_t msg[], const size_t length) {
		if(msg == nullptr) {
			return;
		}

		_clientID = *((uint32_t*)msg);
		msg += sizeof(uint32_t);

		if(length <= sizeof(uint32_t)) {
			return;
		}

		message = std::string((char*)msg);
		msg += message.size() + 1;
	}
} // NMP::Network::Messages
