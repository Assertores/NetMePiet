#pragma once

#include "./screen.hpp"

#include <string>
#include <cstdint>

#include "../network/netcode_api.hpp"

namespace NMP::Screens {

struct StartupScreen : public ScreenI {
	~StartupScreen(void) {}

	// TODO: import defaults
	std::string	_host;
	uint16_t	_port = 13370;

	bool		_connected = false;

	bool		_had_error = false;

	void update(double delta) override;
	bool finished(void) override;

	ScreenI* getNextScreen(void) override;
};

}

