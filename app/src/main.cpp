#include "main.hpp"
#include "IconsMaterialDesign.h"
#include "imgui.h"
#include "imgui_helpers.hpp"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>
#include <stdio.h>

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

static void glfw_error_callback(int error, const char* description) {
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

static ImFont* load_material_design_font(ImGuiIO& io) {
	constexpr float		 fontsize = scale(13.0f);
	static const ImWchar icons_ranges[] = {ICON_MIN_MD, ICON_MAX_16_MD, 0};
	ImFontConfig		 cfg;

	cfg.MergeMode = true;
	cfg.PixelSnapH = true;
	cfg.GlyphOffset.y += fontsize * 0.2f; // centering text

	ImFont* font = io.Fonts->AddFontFromFileTTF(absolute_path(FONT_ICON_FILE_NAME_MD).c_str(), fontsize, &cfg, icons_ranges);
	assert(font);
	return font;
}

ImGui_text		 ImText;
const Constants& consts() {
	static const std::array<std::string, Camera_type::N> cameras =
		{"7004",
		 "7005",
		 "7006"};
	static const size_t	   max_occupant_name_length = 27;
	static const Constants consts = {
		.window_width = 800.0f,
		.window_height = 1280.0f,
		// Number of occupants to list on the frontend
		.n_occupants = 12,
		.occupants = read_occupants(cameras, max_occupant_name_length),
		.camera_numbers = cameras,
		.max_occupant_name_length = max_occupant_name_length,
		.call_timeout = 20000,
	};
	return consts;
}

int main(int, char**) {
	append_to_logfile("Starting up");

	// Setup window
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return 1;

		// Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
	// GL ES 2.0 + GLSL 100
	const char* glsl_version = "#version 100";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
	// GL 3.2 + GLSL 150
	const char* glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);		   // Required on Mac
#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

#ifdef __APPLE__
	// Apple (not me) is multiplying the resolution numbers by 2 somewhere
	const auto window_w = consts().window_width / 2;
	const auto window_h = consts().window_height / 2;
#else
	const auto window_w = consts().window_width;
	const auto window_h = consts().window_height;
#endif
	GLFWwindow* window = glfwCreateWindow(window_w, window_h, "video doorbell by Joppe Koers", NULL, NULL);

	if (window == NULL)
		return 1;
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	io.Fonts->AddFontDefault();

	// Fonts
	ImFont* fonts[] = {io.Fonts->AddFontFromFileTTF(absolute_path("fonts/Roboto-Light.ttf").c_str(), scale(13.0f)),
					   io.Fonts->AddFontFromFileTTF(absolute_path("fonts/Roboto-Bold.ttf").c_str(), scale(13.0f)),
					   load_material_design_font(io)};

	ImText = ImGui_text(fonts);
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		{ // Everything happens in a single imgui window that is the same size as the os window
#ifdef IMGUI_HAS_VIEWPORT
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->GetWorkPos());
			ImGui::SetNextWindowSize(viewport->GetWorkSize());
			ImGui::SetNextWindowViewport(viewport->ID);
#else
			ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
			ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
#endif
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
#define IMGUI_FILL_OS_WINDOW ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize
			ImGui::Begin("document", nullptr, IMGUI_FILL_OS_WINDOW);

			on_frame(); // The actual application is here

			// Cleanup
			ImText.clear_stack();
			ImGui::End();
			ImGui::PopStyleVar(1);
		}
		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
