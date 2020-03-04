#pragma once

//#include <SDL.h>

namespace NMP::Screens {

struct ScreenI {
	//ScreenI(void);
	virtual ~ScreenI(void) {}

	virtual void update(double delta) = 0;
	//virtual void render(void) = 0;
	virtual bool finished(void) = 0;
	//virtual void handleSDL_Event(const SDL_Event& event) = 0;
	virtual ScreenI* getNextScreen(void) = 0;
};

} // NMP

