#include <chrono>
#include <ratio>

#include <SDL.h>
#include <glad/glad.h>
#include <imgui/imgui.h>
#include <imgui/examples/imgui_impl_opengl3.h>
#include <imgui/examples/imgui_impl_sdl.h>

#include <screens/screen.hpp>
#include <screens/startup_screen.hpp>

int main(int argc, char* argv[]) {
	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return 1;
	}

	uint32_t window_flags = 0;
	window_flags |= SDL_WINDOW_OPENGL;
	window_flags |= SDL_WINDOW_SHOWN;
	window_flags |= SDL_WINDOW_RESIZABLE;

	//Use OpenGL 3.3 core
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	SDL_Window* window = SDL_CreateWindow("NetMePiet", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, window_flags);
	if (!window) {
		SDL_Log("Unable to create window: %s", SDL_GetError());

		return 1;
	}

	auto gl_context = SDL_GL_CreateContext(window);
	if (!gl_context) {
		SDL_Log("Unable to create opengl context: %s", SDL_GetError());

		SDL_DestroyWindow(window);
		return 1;
	}

	// glad
	if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
		SDL_Log("Failed to initialize OpenGL context");

		SDL_GL_DeleteContext(gl_context);
		SDL_DestroyWindow(window);
		return 1;
	}

	if (SDL_GL_MakeCurrent(window, gl_context)) {
		SDL_Log("SDL_GL_MakeCurrent(): %s", SDL_GetError());

		SDL_GL_DeleteContext(gl_context);
		SDL_DestroyWindow(window);
		return 1;
	}

	// imgui
	{
		IMGUI_CHECKVERSION();

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		(void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

		ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
		ImGui_ImplOpenGL3_Init();

		// style
		{
			ImGui::StyleColorsDark();

			auto& style = ImGui::GetStyle();
			style.WindowRounding = 2.f;
		}
	}


	auto last_clock = std::chrono::high_resolution_clock::now();
	NMP::Screens::ScreenI* curr_screen = new NMP::Screens::StartupScreen(); // new
	while (curr_screen) {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				delete curr_screen;
				curr_screen = nullptr;
				break;
			}

			ImGui_ImplSDL2_ProcessEvent(&event);
		}
		if (!curr_screen) {
			break;
		}

		auto new_clock = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> delta = new_clock - last_clock;
		last_clock = new_clock;
		curr_screen->update(delta.count());

		// render
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		SDL_GL_SwapWindow(window);
		glClearColor(0.1f, 0.1f, 0.1f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		if (curr_screen->finished()) {
			auto* prev_screen = curr_screen;
			curr_screen = curr_screen->getNextScreen();
			delete prev_screen;
		}
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

