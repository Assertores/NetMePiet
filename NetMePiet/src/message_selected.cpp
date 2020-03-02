#include "network/messages/message_selected.hpp"

//===== ===== EXTERN ===== =====
#include <iostream>

//===== ===== INTERN ===== =====

namespace NMP::Network::Messages {

	size_t Selected::DataSerialize(uint8_t buffer[], const size_t maxSize) const {
		if(buffer == nullptr) {
			return 0;
		}

		size_t payloadSize = sizeof(uint32_t) * 3;
		if(maxSize < payloadSize) {
			return 0;
		}

		*((uint32_t*)buffer) = _clientID;
		buffer += sizeof(uint32_t);

		*((uint32_t*)buffer) = _x;
		buffer += sizeof(uint32_t);

		*((uint32_t*)buffer) = _y;
		buffer += sizeof(uint32_t);

		return payloadSize;
	}

	void Selected::DataDeserialize(const uint8_t msg[], const size_t length) {
		if(msg == nullptr) {
			return;
		}

		_clientID = *((uint32_t*)msg);
		msg += sizeof(uint32_t);

		_x = *((uint32_t*)msg);
		msg += sizeof(uint32_t);

		_y = *((uint32_t*)msg);
		msg += sizeof(uint32_t);
	}

	std::string Selected::ToString(void) {
		std::stringstream ss;

		ss << "Selected | " << _clientID << ": (" << _x << " | " << _y << ")";

		return ss.str();
	}
}
