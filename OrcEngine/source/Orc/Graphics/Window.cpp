#include "Graphics/Window.hpp"

#include <utility>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "Events/KeyboardEvents.hpp"
#include "Events/MouseEvents.hpp"
#include "Events/WindowEvents.hpp"
#include "Input/Keyboard.hpp"
#include "Input/Mouse.hpp"

namespace orc {

namespace {

constexpr int OPENGL_MAJOR_VERSION = 4;
constexpr int OPENGL_MINOR_VERSION = 6;

void glfwErrorCallback(int code, const char* description) noexcept
{
	ORC_FATAL("GLFW error occurred...\n\tCode: {}\n\tDescription: {}", code, description);
}

}

Window::Window(const VideoSettings& videoSettings)
{
	m_videoSettings = videoSettings;

	ORC_LOG_INFO("Initializing window...\nWindow info:\n\tResolution: {}x{}\n\tTitle: {}", m_videoSettings.width, m_videoSettings.height, m_videoSettings.title);

	initGLAD();
}

Window::~Window()
{
	ORC_LOG_INFO("Deinitializing window.");
	glfwDestroyWindow(m_glfwWindow);
	glfwTerminate();
}

void Window::display()
{
	glfwPollEvents();
	glfwSwapBuffers(m_glfwWindow);
}

void Window::setTitle(std::string_view title)
{
	m_videoSettings.title = title;
	glfwSetWindowTitle(m_glfwWindow, m_videoSettings.title.c_str());
}

void Window::setEventCallback(Window::EventCallback eventCallback)
{
	m_videoSettings.eventCallback = std::move(eventCallback);
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
	return {m_videoSettings.width, m_videoSettings.height};
}

uint32_t Window::getFramebufferWidth() const
{
	return m_framebufferWidth;
}

uint32_t Window::getFramebufferHeight() const
{
	return m_framebufferHeight;
}

Vector2u Window::getFramebufferSize() const
{
	return {m_framebufferWidth, m_framebufferHeight};
}

void* Window::getNativeWindow() const
{
	return m_glfwWindow;
}

void Window::updateFramebufferDimensions()
{
	int fbWidth = 0;
	int fbHeight = 0;
	glfwGetFramebufferSize(m_glfwWindow, &fbWidth, &fbHeight);
	m_framebufferWidth = static_cast<uint32_t>(fbWidth);
	m_framebufferHeight = static_cast<uint32_t>(fbHeight);
}

void Window::initGLAD()
{
	ORC_FATAL_CHECK(glfwInit(), "Failed to initialize GLFW.");

	glfwSetErrorCallback(&glfwErrorCallback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_MAJOR_VERSION);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_MINOR_VERSION);

	m_glfwWindow = glfwCreateWindow(static_cast<int>(m_videoSettings.width), static_cast<int>(m_videoSettings.height), m_videoSettings.title.c_str(), nullptr, nullptr);
	ORC_FATAL_CHECK(m_glfwWindow != nullptr, "Failed to create GLFW window.");

	glfwMakeContextCurrent(m_glfwWindow);
	updateFramebufferDimensions();

	glfwSetWindowUserPointer(m_glfwWindow, this);

	ORC_FATAL_CHECK(gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)), "Failed to initialize GLAD.");

	ORC_LOG_INFO("Initialized GLFW & GLAD...\nGraphics info:\n\tOpenGL version: {}.{}\n\tVendor: {}\n\tRenderer: {}", OPENGL_MAJOR_VERSION, OPENGL_MINOR_VERSION, reinterpret_cast<const char*>(glGetString(GL_VENDOR)), reinterpret_cast<const char*>(glGetString(GL_RENDERER)));

	setCallbacks();
	setVsync(m_videoSettings.vsync);
}

void Window::setCallbacks()
{
	glfwSetWindowSizeCallback(m_glfwWindow,
							  [](GLFWwindow* window, int width, int height) noexcept {
								  Window& self = *reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
								  self.m_videoSettings.width = static_cast<uint32_t>(width);
								  self.m_videoSettings.height = static_cast<uint32_t>(height);
								  self.updateFramebufferDimensions();

								  orc::WindowResizedEvent windowResizedEvent(self.m_videoSettings.width, self.m_videoSettings.height);
								  self.m_videoSettings.eventCallback(windowResizedEvent);
							  });

	glfwSetFramebufferSizeCallback(m_glfwWindow,
								   [](GLFWwindow* window, int width, int height) noexcept {
									   Window& self = *reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
									   self.m_framebufferWidth = static_cast<uint32_t>(width);
									   self.m_framebufferHeight = static_cast<uint32_t>(height);

									   orc::WindowResizedEvent windowResizedEvent(self.m_videoSettings.width, self.m_videoSettings.height);
									   self.m_videoSettings.eventCallback(windowResizedEvent);
								   });

	glfwSetWindowCloseCallback(m_glfwWindow,
							   [](GLFWwindow* window) noexcept {
								   Window& self = *reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

								   orc::WindowClosedEvent windowClosedEvent;
								   self.m_videoSettings.eventCallback(windowClosedEvent);
							   });

	glfwSetKeyCallback(m_glfwWindow,
					   [](GLFWwindow* window, int key, int scancode, int action, int mods) noexcept {
						   Window& self = *reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

						   if (action == GLFW_PRESS)
						   {
							   orc::KeyboardKeyPressedEvent keyboardKeyPressedEvent(glfw::glfwKeyToOrcKey(key, mods), glfw::glfwKeyModsToOrcSpecialKeys(mods));
							   self.m_videoSettings.eventCallback(keyboardKeyPressedEvent);
						   }
						   else if (action == GLFW_RELEASE)
						   {
							   orc::KeyboardKeyReleasedEvent keyboardKeyReleasedEvent(glfw::glfwKeyToOrcKey(key, mods), glfw::glfwKeyModsToOrcSpecialKeys(mods));
							   self.m_videoSettings.eventCallback(keyboardKeyReleasedEvent);
						   }
						   else if (action == GLFW_REPEAT)
						   {
						   }
					   });

	glfwSetMouseButtonCallback(m_glfwWindow,
							   [](GLFWwindow* window, int button, int action, int mods) noexcept {
								   Window& self = *reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

								   if (action == GLFW_PRESS)
								   {
									   orc::MouseButtonPressedEvent mouseButtonPressedEvent(glfw::glfwButtonToOrcButton(button));
									   self.m_videoSettings.eventCallback(mouseButtonPressedEvent);
								   }
								   else if (action == GLFW_RELEASE)
								   {
									   orc::MouseButtonReleasedEvent mouseButtonReleasedEvent(glfw::glfwButtonToOrcButton(button));
									   self.m_videoSettings.eventCallback(mouseButtonReleasedEvent);
								   }
							   });

	glfwSetCursorPosCallback(m_glfwWindow,
							 [](GLFWwindow* window, double x, double y) noexcept {
								 Window& self = *reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

								 orc::MouseMovedEvent mouseMovedEvent(static_cast<float>(x), static_cast<float>(y));
								 self.m_videoSettings.eventCallback(mouseMovedEvent);
							 });

	glfwSetScrollCallback(m_glfwWindow,
						  [](GLFWwindow* window, double xDelta, double yDelta) noexcept {
							  Window& self = *reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

							  orc::MouseWheelScrolledEvent mouseWheelScrolledEvent(static_cast<float>(xDelta), static_cast<float>(yDelta));
							  self.m_videoSettings.eventCallback(mouseWheelScrolledEvent);
						  });
}

}
