#include "network/messages/message_pixel.hpp"

//===== ===== EXTERN ===== =====
#include <iostream>

//===== ===== INTERN ===== =====

namespace NMP::Network::Messages {

	size_t Pixel::DataSerialize(uint8_t buffer[], const size_t maxSize) const {
		if(buffer == nullptr) {
			return 0;
		}

		size_t payloadSize = sizeof(uint32_t) * 3;
		if(maxSize < payloadSize) {
			return 0;
		}

		*((uint32_t*)buffer) = _time;
		buffer += sizeof(uint32_t);

		*((uint32_t*)buffer) = _x;
		buffer += sizeof(uint32_t);

		*((uint32_t*)buffer) = _y;
		buffer += sizeof(uint32_t);

		buffer[0] = _r;
		buffer++;

		buffer[0] = _g;
		buffer++;

		buffer[0] = _b;
		buffer++;

		buffer[0] = _a;
		buffer++;

		return payloadSize;
	}

	void Pixel::DataDeserialize(const uint8_t msg[], const size_t length) {
		if(msg == nullptr) {
			return;
		}

		_time = *((uint32_t*)msg);
		msg += sizeof(uint32_t);

		_x = *((uint32_t*)msg);
		msg += sizeof(uint32_t);

		_y = *((uint32_t*)msg);
		msg += sizeof(uint32_t);

		_r = msg[0];
		msg++;

		_g = msg[0];
		msg++;

		_b = msg[0];
		msg++;

		_a = msg[0];
		msg++;
	}

	std::string Pixel::ToString(void) {
		std::stringstream ss;

		ss << "Pixel    | " << "t=" << _time << " (" << _x << " | " << _y << "): " << "r=" << _r << " g=" << _g << " b=" << _b << " a=" << _a;

		return ss.str();
	}
}
