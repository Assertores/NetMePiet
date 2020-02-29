#pragma once

//===== ===== EXTERN ===== =====
#include <string>
#include <cstdint>

//===== ===== INTERN ===== =====
#include "message_base.hpp"

namespace NMP::Network::Messages {
	class Chat final : public Base {
	public:
		Chat() { _type = chat; };

		std::string senderName;
		std::string message;

	protected:
		size_t DataDeserialize(const uint8_t buffer[], const size_t maxSize) override;
		void DataSerialize(const uint8_t msg[], const size_t length) override;
	};
} // NMP::Network::Messages
