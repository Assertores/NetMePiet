#pragma once

//===== ===== EXTERN ===== =====
#include <concurrentqueue.h>
#include <SDL_net.h>

//===== ===== INTERN ===== =====
#include "netcode_types.hpp"


namespace NMP::Network {

	namespace Messages {
		class Base;
	}

	/*!
	 * @brief	all incomming messages, that have to be handlet
	 * @remarks	enqueued by another thread
	 *			the one who dequeues it must delete it
	 */
	InQueue& GetInQueue();

	/*!
	 * @brief		initialices the server
	 * @param port	the listen port of the server
	 * @remars		creates threads
	 */
	int InitServer(uint16_t port = 0000, Context* context = nullptr);

	/*!
	 * @brief			initialices the client
	 * @param hostURL	the url to the host/relay server
	 * @param port		the port, the server lisents at
	 * @remars			creates threads
	 */
	int InitClient(std::string hostURL = "", uint16_t port = 0000, Context* context = nullptr);

	void SetLobbyID(uint32_t newLobbyID, Context* context = nullptr);

	void SetPromiscuousMode(bool mode, Context* context = nullptr);

	/*!
	 * @brief	closes threads
	 * @remars	blocks until all threads a joind
	 */
	void ShutDown(Context* context = nullptr);

	/*!
	 * @brief			sends \link message \endlink
	 * @param message	the message to send, must be created with new
	 */
	void SendMessage(Messages::Base *message, Context* context = nullptr);
} // NMP::Network
