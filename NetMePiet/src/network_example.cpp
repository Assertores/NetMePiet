#pragma once

//===== ===== EXTERN ===== =====
#include <cstdint>
#include <iostream>
#include <thread>
#include <chrono>
#include <map>

//===== ===== INTERN ===== =====
#include "network/netcode_api.hpp"
#include "network/messages/message_base.hpp"
#include "network/messages/message_chat.hpp"
#include "network/messages/message_pixel.hpp"
#include "network/messages/message_selected.hpp"
#include "network/messages/message_size.hpp"
#include "network/messages/message_status.hpp"

namespace NMP::Network {
	inline void ReactToMessage(Messages::Base* m) {
		switch(m->GetMessageType()) {
		case Messages::MessageTypes::heart_beat:
			break;
		case Messages::MessageTypes::chat:
		{
			Messages::Chat* cm = (NMP::Network::Messages::Chat*)m;
			std::cout << cm->_clientID << ": " << cm->_message << std::endl;
			break;
		}
		default:
			break;
		}

		if(m != nullptr) {
			delete(m);
			m = nullptr;
		}
	}

	inline void Example(void) {
		Messages::Chat element;
		element._clientID = 1;
		element._message = "Hello World!";

		uint8_t buffer[1024];
		size_t packageSize = element.Serialize(buffer, 1024);

		//socket.Send(buffer, packageSize);

		//socket.Read(buffer, 1024);

		Messages::Base* m = Messages::Base::Deserialize(buffer, 1024);

		ReactToMessage(m);

		ReactToMessage(nullptr);
	}

	void PrintMessage(Messages::Base* m, std::map<uint32_t, std::string>* names) {
		if(m == nullptr) {
			return;
		}
		if(names == nullptr) {
			return;
		}

		switch(m->GetMessageType()) {
		case Messages::MessageTypes::chat:
		{
			auto cm = (Messages::Chat*)m;
			std::cout << (*names)[cm->_clientID] << ": " << cm->_message << std::endl;
			break;
		}
		case Messages::MessageTypes::pixel:
		{
			auto cm = (Messages::Pixel*)m;
			std::cout << "Pixel (" << cm->_x << " | " << cm->_y << ") was changed at " << cm->_time << "s to (r=" << cm->_r << ", g=" << cm->_g << ", b=" << cm->_b << ", a=" << cm->_a << ")" << std::endl;
			break;
		}
		case Messages::MessageTypes::selected:
		{
			auto cm = (Messages::Selected*)m;
			std::cout << (*names)[cm->_clientID] << " has selected pixel (" << cm->_x << " | " << cm->_y << ")" << std::endl;
			break;
		}
		case Messages::MessageTypes::size:
		{
			auto cm = (Messages::Size*)m;
			std::cout << "Size was changed at " << cm->_time << "s to (" << cm->_width << "x" << cm->_hight << ")" << std::endl;
			break;
		}
		case Messages::MessageTypes::status:
		{
			auto cm = (Messages::Status*)m;
			std::cout << "client: " << (*names)[cm->_clientID] << " has " << (cm->_status == Messages::clientStatus::Connected ? "connected" : "disconnected") << std::endl;
			break;
		}
		default:
			break;
		}
	}

	void Server(void) {
		std::cout << "Started Server" << std::endl;

		InitServer();

		std::this_thread::sleep_for(std::chrono::milliseconds(5000));

		{
			std::map<uint32_t, std::string> names;
			Messages::Base* m;
			while(incomingNetworkMessages.try_dequeue(m)) {
				if(m == nullptr) {
					continue;
				}

				if(m->GetMessageType() == Messages::MessageTypes::status) {
					auto cm = (Messages::Status*)m;

					if(cm->_status == Messages::clientStatus::Connected) {
						names[cm->_clientID] = cm->_name;
					}
				}

				std::cout << "Server:: ";
				PrintMessage(m, &names);

				delete(m);
			}
		}

		ShutDown();
	}

	void Client(uint32_t id) {
		InitClient("127.0.0.1");

		{
			auto m = new Messages::Status();
			m->_clientID = id;
			m->_status = Messages::clientStatus::Connected;
			m->_name = "Andreas-" + std::to_string(id);

			SendMessage(m);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(10));

		{
			auto m = new Messages::Chat();
			m->_clientID = id;
			m->_message = "message from me (" + std::to_string(id) + ")";

			SendMessage(m);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(10));

		{
			auto m = new Messages::Selected();
			m->_clientID = id;
			m->_x = 5 * (id + 1);
			m->_y = 3 * (id + 1);

			SendMessage(m);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(10));

		{
			auto m = new Messages::Size();
			m->_time = (uint32_t)std::chrono::steady_clock::now().time_since_epoch().count();
			m->_width = 5 * (id + 1);
			m->_hight = 3 * (id + 1);

			SendMessage(m);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(10));

		{
			auto m = new Messages::Pixel();
			m->_time = (uint32_t)std::chrono::steady_clock::now().time_since_epoch().count();
			m->_x = 5 * (id + 1);
			m->_y = 3 * (id + 1);
			m->_r = 50 * (id + 1);
			m->_g = 50 * (id + 1);
			m->_b = 50 * (id + 1);
			m->_a = 50 * (id + 1);

			SendMessage(m);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		{
			auto m = new Messages::Status();
			m->_clientID = id;
			m->_status = Messages::clientStatus::Disconnected;

			SendMessage(m);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(100 * (id + 1)));

		{
			std::map<uint32_t, std::string> names;
			Messages::Base* m;
			while(incomingNetworkMessages.try_dequeue(m)) {
				if(m == nullptr) {
					continue;
				}

				if(m->GetMessageType() == Messages::MessageTypes::status) {
					auto cm = (Messages::Status*)m;

					if(cm->_status == Messages::clientStatus::Connected) {
						names[cm->_clientID] = cm->_name;
					}
				}

				std::cout << id << ":: ";
				PrintMessage(m, &names);

				delete(m);
			}
		}


		ShutDown();
	}
} // NMP::Network

int main(int argc, char** argv) {
	if(argc < 2 || argv[1][0] == 's') {
		NMP::Network::Server();
	} else {
		NMP::Network::Client(argv[2][0]);
	}

	return 0;
}
