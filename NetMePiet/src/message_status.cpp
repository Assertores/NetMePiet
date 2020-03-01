#include "network/messages/message_status.hpp"

//===== ===== EXTERN ===== =====
#include <iostream>

//===== ===== INTERN ===== =====

namespace NMP::Network::Messages {

	size_t Status::DataSerialize(uint8_t buffer[], const size_t maxSize) const {
		if(buffer == nullptr) {
			return 0;
		}

		size_t payloadSize = sizeof(uint32_t) + _name.size() + 1 + sizeof(clientStatus); //null character
		if(maxSize < payloadSize) {
			return 0;
		}

		*((uint32_t*)buffer) = _clientID;
		buffer += sizeof(uint32_t);

		memcpy((void*)buffer, _name.data(), _name.size() + 1);
		buffer += _name.size() + 1;

		buffer[0] = _status;
		buffer++;

		return payloadSize;
	}

	void Status::DataDeserialize(const uint8_t msg[], const size_t length) {
		if(msg == nullptr) {
			return;
		}

		_clientID = *((uint32_t*)msg);
		msg += sizeof(uint32_t);

		_name = std::string((char*)msg);
		msg += _name.size() + 1;

		_status = (clientStatus)msg[0];
		msg++;
	}
}
