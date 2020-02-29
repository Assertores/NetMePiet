#include "network/messages/message_base.hpp"

//===== ===== EXTERN ===== =====
#include <iostream>

//===== ===== INTERN ===== =====
#include "network/messages/message_chat.hpp"


using namespace NMP::Network::Messages;

size_t Base::Deserialize(uint8_t buffer[], const size_t maxSize) {

	buffer[4] = this->GetMessageType();
	size_t payloadSize = this->DataDeserialize(buffer + 5, maxSize - 5);
	buffer[0] = (uint32_t)payloadSize + 5;

	return payloadSize + 5;
}

Base* Base::Serialize(const uint8_t msg[], const size_t length) {

	Base* m = nullptr;
	switch(msg[4]) {
	case Messages::heart_beat:
		break;
	case Messages::chat:
		m = new Messages::Chat();
	default:
		break;
	}

	if(m != nullptr) {
		m->DataSerialize(msg + 5, length - 5);
	}

	return m;
}