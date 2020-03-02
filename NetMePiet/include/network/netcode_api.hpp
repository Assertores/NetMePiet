#pragma once

//===== ===== EXTERN ===== =====
#include <concurrentqueue.h>
#include <SDL_net.h>


namespace NMP::Network {

	namespace Messages {
		class Base;
	}

	/*!
	 * @brief	all incomming messages, that have to be handlet
	 * @remarks	enqueued by another thread
	 *			the one who dequeues it must delete it
	 */
	extern moodycamel::ConcurrentQueue<Messages::Base*> incomingNetworkMessages;

	/*!
	 * @brief		initialices the server
	 * @param port	the listen port of the server
	 * @remars		creates threads
	 */
	int InitServer(uint16_t port = 0000);

	/*!
	 * @brief			initialices the client
	 * @param hostURL	the url to the host/relay server
	 * @param port		the port, the server lisents at
	 * @remars			creates threads
	 */
	int InitClient(std::string hostURL = "", uint16_t port = 0000);

	void SetLobbyID(uint32_t newLobbyID);

	void SetPromiscuousMode(bool mode);

	/*!
	 * @brief	closes threads
	 * @remars	blocks until all threads a joind
	 */
	void ShutDown(void);

	/*!
	 * @brief			sends \link message \endlink
	 * @param message	the message to send
	 */
	void SendMessage(Messages::Base *message);
} // NMP::Network
