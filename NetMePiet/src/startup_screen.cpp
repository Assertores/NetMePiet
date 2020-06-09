#include "network/netcode_api.hpp"
#include <screens/startup_screen.hpp>

#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h> // std::string interop

#include <screens/editor_screen.hpp>

#include <iostream>

namespace NMP::Screens {

void StartupScreen::update(double) {
	if (ImGui::Begin("Connect")) {
		ImGui::InputText("host", &_host);
		ImGui::InputScalar("port", ImGuiDataType_U16, &_port);

		if (ImGui::Button("Connect")) {
			//std::cout << "connecting to " << _host << ":" << _port << " ...\n";
			auto ret = NMP::Network::InitClient(_host, _port);
			if (ret == 0) {
				_connected = true;
				_had_error = false;
			} else {
				std::cerr << "error: NMP::Network::InitClient() failed with '" << ret << "'!\n";
				_had_error = true;
			}
		}

		if (_had_error) {
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 0, 0, 0.9), "Error connecting!");
		}
	}
	ImGui::End();
}

bool StartupScreen::finished(void) {
	return _connected;
}

ScreenI* StartupScreen::getNextScreen(void) {
	auto* next_screen = new NMP::Screens::EditorScreen;
	return next_screen;
}

} // NMP::Screens

