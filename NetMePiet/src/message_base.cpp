#include "network/messages/message_base.hpp"

//===== ===== EXTERN ===== =====
#include <cassert>

//===== ===== INTERN ===== =====


using namespace NMP::Network::Messages;

size_t Base::Deserialize(uint8_t buffer[], const size_t maxSize) {
	assert(false && "Not Implemented");

	buffer[4] = this->GetMessageType();
	size_t payloadSize = this->DataDeserialize(buffer + 5, maxSize - 5);
	buffer[0] = (uint32_t)payloadSize + 5;

	return payloadSize + 5;
}

const Base* Base::Serialize(const uint8_t msg[], const size_t maxSize) {
	assert(false && "Not Implemented");
	return nullptr;
}