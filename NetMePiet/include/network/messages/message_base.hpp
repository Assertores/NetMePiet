#pragma once

//===== ===== EXTERN ===== =====
#include <cstdint>

//===== ===== INTERN ===== =====


namespace NMP::Network::Messages {
	enum MessageTypes : uint8_t {
		heart_beat,
		chat,
	};

	class Base {
	public:

		inline const MessageTypes GetMessageType(void) {
			return _type;
		}

		size_t Deserialize(uint8_t buffer[], const size_t maxSize);
		static Base* Serialize(const uint8_t msg[], const size_t maxSize);

	protected:
		virtual size_t DataDeserialize(const uint8_t buffer[], const size_t maxSize) = 0;
		virtual void DataSerialize(const uint8_t msg[], const size_t length) = 0;

		MessageTypes _type = heart_beat;
	};
} // NMP::Network::Messages
