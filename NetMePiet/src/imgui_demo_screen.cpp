#include <screens/imgui_demo_screen.hpp>

#include <imgui/imgui.h>

namespace NMP::Screens {

void ImGuiDemoScreen::update(double) {
	ImGui::ShowDemoWindow();
}

bool ImGuiDemoScreen::finished(void) {
	return false;
}

ScreenI* ImGuiDemoScreen::getNextScreen(void) {
	return nullptr;
}

} // NMP::Screens

