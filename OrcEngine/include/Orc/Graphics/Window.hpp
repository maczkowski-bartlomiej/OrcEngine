#pragma once

#include <cstdint>
#include <functional>
#include <string>

#include "Core/Types.hpp"
#include "Events/Event.hpp"

struct GLFWwindow;

namespace orc {

class Window
{
public:
	using EventCallback = std::function<void(const Event&)>;

	struct VideoSettings
	{
		std::string title = "Orc Engine";

		uint32_t width = 800;
		uint32_t height = 600;

		bool vsync = true;

		EventCallback eventCallback = nullptr;
	};

	explicit Window(const VideoSettings& videoSettings);
	~Window();

	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	Window(Window&&) = delete;
	Window& operator=(Window&&) = delete;

	void display();

	void setTitle(std::string_view title);
	void setEventCallback(EventCallback eventCallback);

	bool getVsync() const;
	void setVsync(bool vsync);

	uint32_t getWidth() const;
	uint32_t getHeight() const;
	Vector2u getSize() const;

	uint32_t getFramebufferWidth() const;
	uint32_t getFramebufferHeight() const;
	Vector2u getFramebufferSize() const;

	void* getNativeWindow() const;

private:
	void initGLAD();
	void setCallbacks();
	void updateFramebufferDimensions();

	VideoSettings m_videoSettings;
	uint32_t m_framebufferWidth = 0;
	uint32_t m_framebufferHeight = 0;
	GLFWwindow* m_glfwWindow = nullptr;
};

}
