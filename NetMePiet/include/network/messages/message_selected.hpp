#pragma once

//===== ===== EXTERN ===== =====
#include <string>
#include <cstdint>

//===== ===== INTERN ===== =====
#include "message_base.hpp"

namespace NMP::Network::Messages {
	class Selected final : public Base {
	public:
		Selected() { _type = selected; _vorwardCompatible = 0; _backwardCompatible = 0; };
		std::string ToString(void) override;

		uint32_t _clientID;
		uint32_t _x;
		uint32_t _y;

	protected:
		size_t DataSerialize(uint8_t buffer[], const size_t maxSize) const override;
		void DataDeserialize(const uint8_t msg[], const size_t length) override;
	};
} // NMP::Network::Messages
