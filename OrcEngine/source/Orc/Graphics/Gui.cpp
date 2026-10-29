#include "Graphics/Gui.hpp"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Core/Debug.hpp"
#include "Graphics/Window.hpp"

namespace orc {

Gui::Gui(Window& window)
	: m_window(window)
{
	ORC_LOG_INFO("Initializing GUI.");

	IMGUI_CHECKVERSION();
	ImGuiContext* context = ImGui::CreateContext();
	ORC_FATAL_CHECK(context != nullptr, "Failed to initialize GUI.");

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	/*
	float fontSize = 18.0f;// *2.0f;
	io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Bold.ttf", fontSize);
	io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Regular.ttf", fontSize);
	*/

	ImGui::StyleColorsDark();
	ORC_FATAL_CHECK(ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(m_window.getNativeWindow()), true), "Failed to initialize ImGUI for GLFW.");
	ORC_FATAL_CHECK(ImGui_ImplOpenGL3_Init("#version 460"), "Failed to initialize ImGUI for OpenGL.");

	ORC_LOG_INFO("GUI:\n\tDear ImGui Version: {}\n\tBackend Platform: {}\n\tBackend Renderer: {}\n\tKeyboard Navigation: {}", IMGUI_VERSION, io.BackendPlatformName ? io.BackendPlatformName : "imgui_impl_glfw", io.BackendRendererName ? io.BackendRendererName : "imgui_impl_opengl3", (io.ConfigFlags & ImGuiConfigFlags_NavEnableKeyboard) ? "Enabled" : "Disabled");
}

Gui::~Gui()
{
	ORC_LOG_INFO("Deinitializing GUI.");

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Gui::begin()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Gui::end()
{
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(static_cast<float>(m_window.getWidth()), static_cast<float>(m_window.getHeight()));

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

}
