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

Window* Window::m_instance = nullptr;

static void glfwErrorCallback(int code, const char* description)
{
	ORC_LOG_ERROR("GLFW error occured\n\tcode: {}\n\tdescription: {}", code, description);
}


Window::Window(const VideoSettings& videoSettings)
	: m_window(nullptr), m_videoSettings(videoSettings)
{
	if (m_instance)
	{
		ORC_FATAL("Window instance already exist!!!");
	}
	else
	{
		ORC_LOG_INFO("Window info...\n\tresolution: {}x{}\n\ttitle: {}", m_videoSettings.width, m_videoSettings.height, m_videoSettings.title);
		m_instance = this;
		initGLAD();
	}
}

Window::~Window() 
{
	if (m_instance == this)
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}
}

void Window::display() 
{
	glfwPollEvents();
	glfwSwapBuffers(m_window);
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

uint32 Window::getWidth() const 
{
	return m_videoSettings.width;
}

uint32 Window::getHeight() const 
{
	return m_videoSettings.height;
}

Vector2u Window::getSize() const 
{
	return Vector2u(m_videoSettings.width, m_videoSettings.height);
}

void* Window::getNativeWindow()
{
	return m_window;
}

void Window::initGLAD()
{
	int errorResult = glfwInit();
	ORC_FATAL_CHECK(errorResult != GLFW_FALSE, "Fatal error occured while initializing GLFW");

	glfwSetErrorCallback(&glfwErrorCallback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_MAJOR_VERSION);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_MINOR_VERSION);

	m_window = glfwCreateWindow(static_cast<int>(m_videoSettings.width), static_cast<int>(m_videoSettings.height), m_videoSettings.title.c_str(), nullptr, nullptr);

	glfwMakeContextCurrent(m_window);
	glfwSetWindowUserPointer(m_window, &m_videoSettings);

	errorResult = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	ORC_FATAL_CHECK(errorResult != NULL, "Fatal error occured while initializing GLAD");

	ORC_LOG_INFO("Graphics info...\n\topenGL version: {}.{}\n\tvendor: {}\n\trenderer: {}", OPENGL_MAJOR_VERSION, OPENGL_MINOR_VERSION, (char*)glGetString(GL_VENDOR), (char*)glGetString(GL_RENDERER));

	setCallbacks();
	setVsync(m_videoSettings.vsync);
}

void Window::setCallbacks()
{
	glfwSetWindowSizeCallback(m_window,
		[](GLFWwindow* window, int width, int height)
		{
			Window::VideoSettings& videoSettings = *reinterpret_cast<Window::VideoSettings*>(glfwGetWindowUserPointer(window));
			videoSettings.width = static_cast<uint32>(width);
			videoSettings.height = static_cast<uint32>(height);

			orc::WindowResizedEvent windowResizedEvent(videoSettings.width, videoSettings.height);
			videoSettings.eventCallback(windowResizedEvent);
		}
	);

	glfwSetFramebufferSizeCallback(m_window,
		[](GLFWwindow* window, int width, int height)
		{
			glViewport(0, 0, width, height);
		}
	);

	glfwSetWindowCloseCallback(m_window,
		[](GLFWwindow* window)
		{
			Window::VideoSettings& videoSettings = *reinterpret_cast<Window::VideoSettings*>(glfwGetWindowUserPointer(window));

			orc::WindowClosedEvent windowClosedEvent;
			videoSettings.eventCallback(windowClosedEvent);
		}
	);

	glfwSetKeyCallback(m_window,
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

	glfwSetMouseButtonCallback(m_window,
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

	glfwSetCursorPosCallback(m_window,
		[](GLFWwindow* window, double x, double y)
		{
			Window::VideoSettings& properties = *reinterpret_cast<Window::VideoSettings*>(glfwGetWindowUserPointer(window));

			orc::MouseMovedEvent mouseMovedEvent(static_cast<float>(x), static_cast<float>(y));
			properties.eventCallback(mouseMovedEvent);
		}
	);

	glfwSetScrollCallback(m_window,
		[](GLFWwindow* window, double xDelta, double yDelta)
		{
			Window::VideoSettings& properties = *reinterpret_cast<Window::VideoSettings*>(glfwGetWindowUserPointer(window));

			orc::MouseWheelScrolledEvent mouseWheelScrolledEvent(static_cast<float>(xDelta), static_cast<float>(yDelta));
			properties.eventCallback(mouseWheelScrolledEvent);
		}
	);
}

}
