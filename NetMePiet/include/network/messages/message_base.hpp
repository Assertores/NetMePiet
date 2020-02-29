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

		/*!
		 * @return	the type of the message
		 */
		inline const MessageTypes GetMessageType(void) const {
			if(this == nullptr) {
				return heart_beat;
			}

			return _type;
		}

		/*!
		 * @brief			converts a object to a Datastream
		 * @param buffer	the memory, the data is written to
		 * @param maxSize	the maximum size of data that can be written to the \link buffer \endlink
		 * @return			the size of the package
		 */
		size_t Serialize(uint8_t buffer[], const size_t maxSize) const;

		/*!
		 * @brief			creates a object out of an Datastream with new
		 * @param msg		the data
		 * @param length	the length of the data \link msg \endlink
		 * @return			a with new created Message object casted down to Base
		 * @remarks			kan be nullptr if it is eather heart_beat or unknown
		 */
		static Base* Deserialize(const uint8_t msg[], const size_t length);

	protected:
		/*!
		 * @brief			converts a object to a Datastream
		 * @param buffer	the memory, the data is written to
		 * @param maxSize	the maximum size of data that can be written to the \link buffer \endlink
		 * @return			the size of the package
		 */
		virtual size_t DataSerialize(const uint8_t buffer[], const size_t maxSize) const = 0;
		
		/*!
		 * @brief			creates a object out of an Datastream
		 * @param msg		the data
		 * @param length	the length of the data \link msg \endlink
		 * @return			a with new created Message object casted down to Base
		 * @remarks			kan be nullptr if it is eather heart_beat or unknown
		 */
		virtual void DataDeserialize(const uint8_t msg[], const size_t length) = 0;

		MessageTypes _type = heart_beat; //!< the message type pleace overwrite
	};
} // NMP::Network::Messages
