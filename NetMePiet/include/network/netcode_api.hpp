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
	moodycamel::ConcurrentQueue<Messages::Base*> incomingNetworkMessages;

	/*!
	 * @brief	initialices the server
	 * @remars	creates threads
	 */
	int InitServer(void);

	/*!
	 * @brief	initialices the client
	 * @remars	creates threads
	 */
	int InitClient(void);

	/*!
	 * @brief	closes threads
	 * @remars	blocks until all threads a joind
	 */
	void ShutDown(void);

	/*!
	 * @brief			sends \link message \endlink to \link socket \endlink
	 * @param message	the message to send
	 * @param socket	the socket to send it through
	 * @remarks			if nullptr server socket will be used
	 * @return			if the message has been send
	 */
	bool SendMessage(Messages::Base *message, TCPsocket socket = nullptr);
} // NMP::Network
