#include "OrcPch.hpp"
#include "Graphics/Gui.hpp"
#include "Engine/Debug.hpp"
#include "Engine/Engine.hpp"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace orc {

bool Gui::init()
{
	ORC_LOG_INFO("Initializing GUI...");

	IMGUI_CHECKVERSION();
	ImGuiContext* context = ImGui::CreateContext();
	if (!context)
	{
		ORC_LOG_FATAL("Failed to initialize GUI");
		return false;
	}

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	/*
	float fontSize = 18.0f;// *2.0f;
	io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Bold.ttf", fontSize);
	io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Regular.ttf", fontSize);
	*/

	ImGui::StyleColorsDark();
	if (!ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(Engine::get().getWindow().getNativeWindow()), true))
	{
		ORC_LOG_FATAL("Failed to initialize ImGUI for GLFW");
		return false;
	}

	if (!ImGui_ImplOpenGL3_Init("#version 460"))
	{
		ORC_LOG_FATAL("Failed to initialize ImGUI for OpenGL");
		return false;
	}

	return true;
}

void Gui::deinit()
{
	ORC_LOG_INFO("Deinitializing GUI...");

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
	Engine& engine = Engine::get();
	io.DisplaySize = ImVec2(static_cast<float>(engine.getWindow().getWidth()), static_cast<float>(engine.getWindow().getHeight()));

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

}
