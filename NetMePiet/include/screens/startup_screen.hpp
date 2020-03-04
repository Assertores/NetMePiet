#pragma once

#include "./screen.hpp"

namespace NMP::Screens {

struct StartupScreen : public ScreenI {
	~StartupScreen(void) {}

	void update(double delta) override;
	bool finished(void) override;

	ScreenI* getNextScreen(void) override;
};

}

