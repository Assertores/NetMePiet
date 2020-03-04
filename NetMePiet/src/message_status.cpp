#include "network/messages/message_status.hpp"

//===== ===== EXTERN ===== =====
#include <iostream>
#include <cstring>

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

		buffer[0] = _status;
		buffer++;

		if(status == Connected) {
			memcpy((void*)buffer, _name.data(), _name.size() + 1);
			buffer += _name.size() + 1;
		}

		return payloadSize;
	}

	void Status::DataDeserialize(const uint8_t msg[], const size_t length) {
		if(msg == nullptr) {
			return;
		}

		_clientID = *((uint32_t*)msg);
		msg += sizeof(uint32_t);

		_status = (clientStatus)msg[0];
		msg++;

		if(_status == Connected) {
			_name = std::string((char*)msg);
			msg += _name.size() + 1;
		}
	}

	std::string Status::ToString(void) {
		std::stringstream ss;

		ss << "Status   | " << _clientID << ": ";

		switch(_status) {
		case NMP::Network::Messages::Connected:
			ss << "Connected, name=" << _name << ", to lobby " << _lobbyID;
			break;
		case NMP::Network::Messages::Disconnected:
			ss << "Disconnected";
			break;
		default:
			break;
		}

		return ss.str();
	}
}
