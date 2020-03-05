#pragma once

#include "./screen.hpp"

namespace NMP::Screens {

struct ImGuiDemoScreen : public ScreenI {
	~ImGuiDemoScreen(void) {}

	void update(double delta) override;
	bool finished(void) override;

	ScreenI* getNextScreen(void) override;
};

}

