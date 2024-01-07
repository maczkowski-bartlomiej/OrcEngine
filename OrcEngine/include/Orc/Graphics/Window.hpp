#pragma once

#include "Engine/Core.hpp"
#include "Events/Event.hpp"

#include <string>
#include <functional>

struct GLFWwindow;

namespace orc {

class Window
{
public:
	using EventCallback = std::function<void(Event&)>;

	struct VideoSettings
	{
		std::string title = "Orc Engine";

		uint32_t width = 800;
		uint32_t height = 600;

		bool vsync = true;

		EventCallback eventCallback = nullptr;
	};

	Window(const VideoSettings& properties);
	~Window();

	void display();
	
	void setTitle(const std::string& title);
	void setEventCallback(EventCallback eventCallback);

	bool getVsync() const;
	void setVsync(bool vsync);

	uint32_t getWidth() const;
	uint32_t getHeight() const;
	Vector2u getSize() const;

	void* getNativeWindow();

private:
	void initGLAD();
	void setCallbacks();

	GLFWwindow* m_window;
	VideoSettings m_videoSettings;

	static Window* m_instance;
};

}
