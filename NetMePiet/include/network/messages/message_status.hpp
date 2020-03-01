#pragma once

//===== ===== EXTERN ===== =====
#include <string>
#include <cstdint>

//===== ===== INTERN ===== =====
#include "message_base.hpp"

namespace NMP::Network::Messages {
	enum clientStatus : uint8_t {
		Connected,
		Disconnected,
	};

	class Status final : public Base {
	public:
		Status() { _type = status; _vorwardCompatible = 0; _backwardCompatible = 0; };

		uint32_t _clientID;
		std::string _name;
		clientStatus _status;

	protected:
		size_t DataSerialize(uint8_t buffer[], const size_t maxSize) const override;
		void DataDeserialize(const uint8_t msg[], const size_t length) override;
	};
} // NMP::Network::Messages
