#include "network/messages/message_size.hpp"

//===== ===== EXTERN ===== =====
#include <iostream>

//===== ===== INTERN ===== =====

namespace NMP::Network::Messages {

	size_t Size::DataSerialize(uint8_t buffer[], const size_t maxSize) const {
		if(buffer == nullptr) {
			return 0;
		}

		size_t payloadSize = sizeof(uint32_t) * 3;
		if(maxSize < payloadSize) {
			return 0;
		}

		*((uint32_t*)buffer) = _time;
		buffer += sizeof(uint32_t);

		*((uint32_t*)buffer) = _width;
		buffer += sizeof(uint32_t);

		*((uint32_t*)buffer) = _hight;
		buffer += sizeof(uint32_t);

		return payloadSize;
	}

	void Size::DataDeserialize(const uint8_t msg[], const size_t length) {
		if(msg == nullptr) {
			return;
		}

		_time = *((uint32_t*)msg);
		msg += sizeof(uint32_t);

		_width = *((uint32_t*)msg);
		msg += sizeof(uint32_t);

		_hight = *((uint32_t*)msg);
		msg += sizeof(uint32_t);
	}

	std::string Size::ToString(void) {
		std::stringstream ss;

		ss << "Size     | " << "t=" << _time << ": (" << _width << " | " << _hight << ")";

		return ss.str();
	}
}
