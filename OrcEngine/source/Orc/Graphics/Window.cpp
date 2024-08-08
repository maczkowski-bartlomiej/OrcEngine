#include "OrcPch.hpp"

#include "Graphics/Window.hpp"

#include "Events/MouseEvents.hpp"
#include "Events/WindowEvents.hpp"
#include "Events/KeyboardEvents.hpp"

#include "Input/Mouse.hpp"
#include "Input/Keyboard.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

constexpr int OPENGL_MAJOR_VERSION = 4;
constexpr int OPENGL_MINOR_VERSION = 6;

namespace orc {

static void glfwErrorCallback(int code, const char* description)
{
	ORC_LOG_ERROR("GLFW error occured\n\tCode: {}\n\tDescription: {}", code, description);
}

bool Window::init(const VideoSettings& videoSettings)
{
	m_videoSettings = videoSettings;

	ORC_LOG_INFO("Initializing window...");
	ORC_LOG_INFO("Window info...\n\tResolution: {}x{}\n\tTitle: {}", m_videoSettings.width, m_videoSettings.height, m_videoSettings.title);

	return initGLAD();
}

void Window::deinit()
{
	ORC_LOG_INFO("Deinitializing window...");
	glfwDestroyWindow(m_glfwWindow);
	glfwTerminate();
}

void Window::display() 
{
	glfwPollEvents();
	glfwSwapBuffers(m_glfwWindow);
}

void Window::setTitle(const std::string& title)
{
	m_videoSettings.title = title;
	glfwSetWindowTitle(m_glfwWindow, title.c_str());
}

void Window::setEventCallback(Window::EventCallback eventCallback) 
{
	m_videoSettings.eventCallback = eventCallback;
}

bool Window::getVsync() const 
{
	return m_videoSettings.vsync;
}

void Window::setVsync(bool vsync) 
{
	if (vsync)
	{
		glfwSwapInterval(1);
	}
	else
	{
		glfwSwapInterval(0);
	}

	m_videoSettings.vsync = vsync;
}

uint32_t Window::getWidth() const 
{
	return m_videoSettings.width;
}

uint32_t Window::getHeight() const 
{
	return m_videoSettings.height;
}

Vector2u Window::getSize() const 
{
	return Vector2u(m_videoSettings.width, m_videoSettings.height);
}

void* Window::getNativeWindow() const
{
	return m_glfwWindow;
}

bool Window::initGLAD()
{
	int errorResult = glfwInit();
	if (!errorResult)
	{
		ORC_LOG_FATAL("Failed to initialize GLFW");
		return false;
	}

	glfwSetErrorCallback(&glfwErrorCallback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_MAJOR_VERSION);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_MINOR_VERSION);

	m_glfwWindow = glfwCreateWindow(static_cast<int>(m_videoSettings.width), static_cast<int>(m_videoSettings.height), m_videoSettings.title.c_str(), nullptr, nullptr);

	glfwMakeContextCurrent(m_glfwWindow);
	glfwSetWindowUserPointer(m_glfwWindow, &m_videoSettings);

	errorResult = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	if (!errorResult)
	{
		ORC_LOG_FATAL("Failed to initialize GLAD");
		return false;
	}

	ORC_LOG_INFO("Graphics info...\n\tOpenGL version: {}.{}\n\tVendor: {}\n\tRenderer: {}", OPENGL_MAJOR_VERSION, OPENGL_MINOR_VERSION, (char*)glGetString(GL_VENDOR), (char*)glGetString(GL_RENDERER));

	setCallbacks();
	setVsync(m_videoSettings.vsync);

	return true;
}

void Window::setCallbacks()
{
	glfwSetWindowSizeCallback(m_glfwWindow,
		[](GLFWwindow* window, int width, int height)
		{
			Window::VideoSettings& videoSettings = *reinterpret_cast<Window::VideoSettings*>(glfwGetWindowUserPointer(window));
			videoSettings.width = static_cast<uint32_t>(width);
			videoSettings.height = static_cast<uint32_t>(height);

			orc::WindowResizedEvent windowResizedEvent(videoSettings.width, videoSettings.height);
			videoSettings.eventCallback(windowResizedEvent);
		}
	);

	glfwSetFramebufferSizeCallback(m_glfwWindow,
		[](GLFWwindow* window, int width, int height)
		{
			glViewport(0, 0, width, height);
		}
	);

	glfwSetWindowCloseCallback(m_glfwWindow,
		[](GLFWwindow* window)
		{
			Window::VideoSettings& videoSettings = *reinterpret_cast<Window::VideoSettings*>(glfwGetWindowUserPointer(window));

			orc::WindowClosedEvent windowClosedEvent;
			videoSettings.eventCallback(windowClosedEvent);
		}
	);

	glfwSetKeyCallback(m_glfwWindow,
		[](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			Window::VideoSettings& videoSettings = *reinterpret_cast<Window::VideoSettings*>(glfwGetWindowUserPointer(window));

			if (action == GLFW_PRESS)
			{
				orc::KeyboardKeyPressedEvent keyboardKeyPressedEvent(glfw::glfwKeyToOrcKey(key, mods), glfw::glfwKeyModsToOrcSpecialKeys(mods));
				videoSettings.eventCallback(keyboardKeyPressedEvent);
			}
			else if (action == GLFW_RELEASE)
			{
				orc::KeyboardKeyReleasedEvent keyboardKeyReleasedEvent(glfw::glfwKeyToOrcKey(key, mods), glfw::glfwKeyModsToOrcSpecialKeys(mods));
				videoSettings.eventCallback(keyboardKeyReleasedEvent);
			}
			else if (action == GLFW_REPEAT)
			{
			}
		}
	);

	glfwSetMouseButtonCallback(m_glfwWindow,
		[](GLFWwindow* window, int button, int action, int mods)
		{
			Window::VideoSettings& properties = *reinterpret_cast<Window::VideoSettings*>(glfwGetWindowUserPointer(window));

			if (action == GLFW_PRESS)
			{
				orc::MouseButtonPressedEvent mouseButtonPressedEvent(glfw::glfwButtonToOrcButton(button));
				properties.eventCallback(mouseButtonPressedEvent);
			}
			else if (action == GLFW_RELEASE)
			{
				orc::MouseButtonReleasedEvent mouseButtonReleasedEvent(glfw::glfwButtonToOrcButton(button));
				properties.eventCallback(mouseButtonReleasedEvent);
			}
		}
	);

	glfwSetCursorPosCallback(m_glfwWindow,
		[](GLFWwindow* window, double x, double y)
		{
			Window::VideoSettings& properties = *reinterpret_cast<Window::VideoSettings*>(glfwGetWindowUserPointer(window));

			orc::MouseMovedEvent mouseMovedEvent(static_cast<float>(x), static_cast<float>(y));
			properties.eventCallback(mouseMovedEvent);
		}
	);

	glfwSetScrollCallback(m_glfwWindow,
		[](GLFWwindow* window, double xDelta, double yDelta)
		{
			Window::VideoSettings& properties = *reinterpret_cast<Window::VideoSettings*>(glfwGetWindowUserPointer(window));

			orc::MouseWheelScrolledEvent mouseWheelScrolledEvent(static_cast<float>(xDelta), static_cast<float>(yDelta));
			properties.eventCallback(mouseWheelScrolledEvent);
		}
	);
}

}
