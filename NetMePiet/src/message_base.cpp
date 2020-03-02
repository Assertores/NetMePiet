#include "network/messages/message_base.hpp"

//===== ===== EXTERN ===== =====
#include <iostream>

//===== ===== INTERN ===== =====
#include "network/messages/message_chat.hpp"


namespace NMP::Network::Messages {

	size_t Base::Serialize(uint8_t buffer[], const size_t maxSize) const {

		buffer[0] = this->GetMessageType();
		buffer[1] = this->_vorwardCompatible;
		buffer[2] = this->_backwardCompatible;
		buffer += 3;
		*((uint32_t*)buffer) = _lobbyID;
		buffer += sizeof(uint32_t);
		size_t payloadSize = this->DataSerialize(buffer, maxSize - 3 - sizeof(uint32_t));

		return payloadSize;
	}

	Base* Base::Deserialize(uint8_t msg[], const size_t length) {

		Base* m = nullptr;
		switch(msg[0]) {
		case Messages::heart_beat:
			break;
		case Messages::chat:
			m = new Messages::Chat();
		default:
			break;
		}

		if(m != nullptr) {
			if(msg[1] > m->_vorwardCompatible) {
				//TODO: error not compatible
				return nullptr;
			} else {
				m->_lobbyID = *((uint32_t*)(msg + 3));

				for(uint8_t i = msg[2]; i < m->_backwardCompatible; i++) {
					m->UpdateVersion(msg + 3 + sizeof(uint32_t), i);
				}

				m->DataDeserialize(msg + 3 + sizeof(uint32_t), length - 3 - sizeof(uint32_t));
			}
		}

		return m;
	}
}
