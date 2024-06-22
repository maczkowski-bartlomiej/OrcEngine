#include "OrcPch.hpp"
#include "Graphics/Gui.hpp"

#include "Engine/Engine.hpp"

#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

namespace orc {

Gui* Gui::m_instance = nullptr;

Gui::Gui()
{
	if (m_instance)
	{
		ORC_FATAL("Gui instance already exist!!!");
		return;
	}

	m_instance = this;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	/*
	float fontSize = 18.0f;// *2.0f;
	io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Bold.ttf", fontSize);
	io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Regular.ttf", fontSize);
	*/

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(Engine::get().getWindow().getNativeWindow()), true);
	ImGui_ImplOpenGL3_Init("#version 460");
}

Gui::~Gui()
{
	if (m_instance == this)
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
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
