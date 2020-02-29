#include "network/messages/message_base.hpp"

//===== ===== EXTERN ===== =====
#include <iostream>

//===== ===== INTERN ===== =====
#include "network/messages/message_chat.hpp"


using namespace NMP::Network::Messages;

size_t Base::Serialize(uint8_t buffer[], const size_t maxSize) const {

	buffer[0] = this->GetMessageType();
	size_t payloadSize = this->DataSerialize(buffer + 1, maxSize - 1);

	return payloadSize;
}

Base* Base::Deserialize(const uint8_t msg[], const size_t length) {

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
		m->DataDeserialize(msg + 1, length - 1);
	}

	return m;
}