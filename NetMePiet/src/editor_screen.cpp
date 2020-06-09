#include <screens/editor_screen.hpp>

#include <imgui/imgui.h>

namespace NMP::Screens {

void EditorScreen::update(double) {
	ImGui::ShowDemoWindow();
}

bool EditorScreen::finished(void) {
	return false;
}

ScreenI* EditorScreen::getNextScreen(void) {
	return nullptr;
}

} // NMP::Screens

