#pragma once

//===== ===== EXTERN ===== =====
#include <string>
#include <cstdint>

//===== ===== INTERN ===== =====
#include "message_base.hpp"

namespace NMP::Network::Messages {
	class Pixel final : public Base {
	public:
		Pixel() { _type = pixel; _vorwardCompatible = 0; _backwardCompatible = 0; };
		std::string ToString(void) override;

		uint32_t _time;
		uint32_t _x;
		uint32_t _y;
		uint8_t _r;
		uint8_t _g;
		uint8_t _b;
		uint8_t _a;


	protected:
		size_t DataSerialize(uint8_t buffer[], const size_t maxSize) const override;
		void DataDeserialize(const uint8_t msg[], const size_t length) override;
	};
} // NMP::Network::Messages
