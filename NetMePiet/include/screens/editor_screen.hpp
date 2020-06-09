#pragma once

#include "./screen.hpp"

namespace NMP::Screens {

struct EditorScreen : public ScreenI {
	~EditorScreen(void) {}

	void update(double delta) override;
	bool finished(void) override;

	ScreenI* getNextScreen(void) override;
};

}

