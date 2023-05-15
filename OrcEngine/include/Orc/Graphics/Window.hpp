#pragma once

#include "Events/Event.hpp"
#include "System/System.hpp"
#include "Math/Vector2.hpp"

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
		std::string title = "orc Engine";

		uint32 width = 800u;
		uint32 height = 600u;

		bool vsync = true;

		EventCallback eventCallback = nullptr;
	};

	Window(const VideoSettings& properties);
	~Window();

	void display();
	
	void setEventCallback(EventCallback eventCallback);

	bool getVsync() const;
	void setVsync(bool vsync);

	uint32 getWidth() const;
	uint32 getHeight() const;
	Vector2u getSize() const;

	void* getNativeWindow();

private:
	void initGLAD();
	void setCallbacks();

	GLFWwindow* m_window = nullptr;
	VideoSettings m_videoSettings;

	static uint32 m_windowCount;
};

}
