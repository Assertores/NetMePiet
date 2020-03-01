//===== ===== EXTERN ===== =====
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <concurrentqueue.h>

//===== ===== INTERN ===== =====
#include "network/netcode_api.hpp"
#include "network/messages/message_base.hpp"


#define FAILEDINPUT			"your input Was denied please try again: "

#define COMMAND_EXIT		"exit"
#define COMMAND_SEXIT		"e"
#define COMMAND_QUIT		"quit"
#define COMMAND_SQUIT		"q"
#define COMMAND_HELP		"help"
#define COMMAND_SHELP		"h"
#define COMMAND_QUEUE		"dequeue"
#define COMMAND_SQUEUE		"d"
#define COMMAND_CLEAR		"clear"
#define COMMAND_SCLEAR		"c"

#define HELP_QUIT \
"syntax: \"exit\", \"e\", \"quit\", \"q\" | quits the programm."
#define HELP_QUEUE \
"syntax: \"dequeue\", \"d\" | prints the bufferd commands sins last command"
#define HELP_CLEAR \
"syntax: \"clear\", \"c\"           | clears the console"
#define HELP_HELP \
"syntax: \"help [command]\"       | prints the help to the [command]\n"\
"\"help\", \"h\"                    | prints this help\n"\
"\"exit\", \"e\", \"quit\", \"q\"       | quits the programm\n"\
"\"dequeue\", \"d\"                   | dequeues the commands\n"\
"\"clear\", \"c\"                     | clers the console\n"\

#define COM_NOT_FOUND		"command not found type \"help\" for more informations."
#define NOT_ENOUGH_PARAMETER	"not enough parameter"


void PrintQueue(void);

void ClearConsole(void);

int main(int argc, char** argv) {
	int port = 13370;
	if(argc >= 2) {
		port = std::stoi(argv[1]);
	}

	std::cout << "[Server] start server at port: " << port << "." << std::endl;
	NMP::Network::InitServer(port);
	std::cout << "[Server] is listening." << std::endl;

	//===== ===== Programm Loop ===== =====

	bool exit = false;
	std::string inputBuffer;
	std::vector<std::string> input;

	while(!exit) {
		//----- ----- input ----- -----
		input.clear();

		// eg CTRL-D in bash
		if(std::cin.eof()) {
			exit = true;
			std::cout << '\n';
			break;
		}

		std::cout << "> ";

		std::getline(std::cin, inputBuffer);
		if(inputBuffer.empty())
			continue;

		std::istringstream tmp(inputBuffer);
		while(tmp.good()) {
			inputBuffer.clear();
			tmp >> inputBuffer;
			input.push_back(inputBuffer);
		}

/*
#ifdef _WIN32
		system("cls");
#else
		system("clear");
#endif
*/

		//----- ----- Logic and Rendering ----- -----

		if(input.size() <= 0) {
			std::cout << HELP_HELP << std::endl;
			continue;
		}
		if(input[0] == COMMAND_EXIT ||
		   input[0] == COMMAND_SEXIT ||
		   input[0] == COMMAND_QUIT ||
		   input[0] == COMMAND_SQUIT) {
			exit = true;
		} else if(input[0] == COMMAND_HELP ||
				  input[0] == COMMAND_SHELP) {
			if(input.size() <= 1) {
				std::cout << HELP_HELP << std::endl;
			} else if(input[1] == COMMAND_EXIT ||
					  input[1] == COMMAND_SEXIT ||
					  input[1] == COMMAND_QUIT ||
					  input[1] == COMMAND_SQUIT) {
				std::cout << HELP_QUIT << std::endl;
			} else if(input[1] == COMMAND_QUEUE ||
					  input[1] == COMMAND_SQUEUE) {
				std::cout << HELP_QUEUE << std::endl;
			} else if(input[1] == COMMAND_CLEAR ||
					  input[1] == COMMAND_SCLEAR) {
				std::cout << HELP_CLEAR << std::endl;
			} else {
				std::cout << HELP_HELP << std::endl;
			}

		} else if(input[0] == COMMAND_QUEUE ||
				  input[0] == COMMAND_SQUEUE) {
			PrintQueue();
		} else if(input[0] == COMMAND_CLEAR ||
				  input[0] == COMMAND_SCLEAR) {
			ClearConsole();
		} else {
			std::cout << COM_NOT_FOUND << std::endl;
		}
	}

	std::cout << "[SERVER] stops" << std::endl;
	NMP::Network::ShutDown();

	return 0;
}

void PrintQueue(void) {
	bool hasDequeued = false;
	NMP::Network::Messages::Base* m;
	while(NMP::Network::incomingNetworkMessages.try_dequeue(m)) {
		if(m == nullptr) {
			continue;
		}

		hasDequeued = true;

		std::cout << m->ToString() << std::endl;

		delete(m);
	}

	if(!hasDequeued) {
		std::cout << "[SERVER] nothing to dequeue" << std::endl;
	}
}

void ClearConsole(void) {
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}
