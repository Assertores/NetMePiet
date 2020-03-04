#include <screens/startup_screen.hpp>

#include <imgui/imgui.h>

namespace NMP::Screens {

void StartupScreen::update(double delta) {
	ImGui::ShowDemoWindow();
}

bool StartupScreen::finished(void) {
	return false;
}

ScreenI* StartupScreen::getNextScreen(void) {
	return nullptr;
}

} // NMP::Screens

