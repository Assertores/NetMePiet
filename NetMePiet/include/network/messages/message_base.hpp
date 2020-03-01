#pragma once

//===== ===== EXTERN ===== =====
#include <cstddef>
#include <cstdint>

//===== ===== INTERN ===== =====


namespace NMP::Network::Messages {
	enum MessageTypes : uint8_t {
		heart_beat,
		chat,
		pixel,
		selected,
		size,
		status,
	};

	class Base {
	public:

		/*!
		 * @return	the type of the message
		 */
		inline MessageTypes GetMessageType(void) const {
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
		static Base* Deserialize(uint8_t msg[], const size_t length);

	protected:
		/*!
		 * @brief			converts a object to a Datastream
		 * @param buffer	the memory, the data is written to
		 * @param maxSize	the maximum size of data that can be written to the \link buffer \endlink
		 * @return			the size of the package
		 */
		virtual size_t DataSerialize(uint8_t buffer[], const size_t maxSize) const = 0;
		
		/*!
		 * @brief			creates a object out of an Datastream
		 * @param msg		the data
		 * @param length	the length of the data \link msg \endlink
		 * @return			a with new created Message object casted down to Base
		 * @remarks			kan be nullptr if it is eather heart_beat or unknown
		 */
		virtual void DataDeserialize(const uint8_t msg[], const size_t length) = 0;

		/*!
		 * @brief				updates the \link msg \endlink array to a format the next version will understand
		 * @param msg			the message that has to be new formated
		 * @param oldVersion	the version \link msg \endlink is currently at
		 * @return				whether the version update was sucsessfull
		 */
		virtual bool UpdateVersion(uint8_t msg[], uint8_t oldVersion) {
			return false;
		}

		uint8_t _vorwardCompatible; //!< increas this if the old \link DataDeserialize \endlink can't understand the stream from the current \link DataSerialize \endlink.
		uint8_t _backwardCompatible; //!< increase this if the old \link DataDeserialize \endlink can still understand the stream from the current \link DataSerialize \endlink.
		MessageTypes _type = heart_beat; //!< the message type pleace overwrite
	};
} // NMP::Network::Messages
