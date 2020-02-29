#include "network/messages/message_chat.hpp"

//===== ===== EXTERN ===== =====
#include <iostream>

//===== ===== INTERN ===== =====

using namespace NMP::Network::Messages;

size_t Chat::DataDeserialize(const uint8_t buffer[], const size_t maxSize) {
	if(buffer == nullptr) {
		return 0;
	}

	size_t payloadSize = senderName.size() + 1 + message.size() + 1; //null character
	if(maxSize < payloadSize) {
		return 0;
	}

	memcpy((void*)buffer, senderName.data(), senderName.size() + 1);
	buffer += senderName.size() + 1;

	memcpy((void*)buffer, message.data(), message.size() + 1);
	buffer += message.size() + 1;

	return payloadSize;
}

void Chat::DataSerialize(const uint8_t msg[], const size_t length) {
	if(msg == nullptr) {
		return;
	}

	senderName = std::string((char*)msg);
	msg += senderName.size() + 1;

	if(length <= senderName.size() + 1) {
		return;
	}

	message = std::string((char*)msg);
	msg += message.size() + 1;

	if(length <= senderName.size() + 1 + message.size() + 1) {
		std::cout << "message to long: " << length << " <-> " + senderName.size() + 1 + message.size() + 1 << std::endl;
	}
}