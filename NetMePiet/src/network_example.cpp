#pragma once

//===== ===== EXTERN ===== =====
#include <cstdint>
#include <iostream>

//===== ===== INTERN ===== =====
#include "network/messages/message_base.hpp"
#include "network/messages/message_chat.hpp"

namespace NMP::Network {
	inline void ReactToMessage(Messages::Base* m) {
		switch(m->GetMessageType()) {
		case Messages::MessageTypes::heart_beat:
			break;
		case Messages::MessageTypes::chat:
		{
			Messages::Chat* cm = (NMP::Network::Messages::Chat*)m;
			std::cout << cm->senderName << ": " << cm->message << std::endl;
			break;
		}
		default:
			break;
		}

		if(m != nullptr) {
			delete(m);
			m = nullptr;
		}
	}

	inline void Example() {
		Messages::Chat element;
		element.senderName = "Andreas";
		element.message = "Hello World!";

		uint8_t buffer[1024];
		size_t packageSize = element.Deserialize(buffer, 1024);

		//socket.Send(buffer, packageSize);

		//socket.Read(buffer, 1024);

		Messages::Base* m = Messages::Base::Serialize(buffer, 1024);

		ReactToMessage(m);

		ReactToMessage(nullptr);
	}
} // NMP::Network
