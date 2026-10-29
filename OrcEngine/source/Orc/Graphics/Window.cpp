#include "Graphics/Window.hpp"

#include <utility>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "Events/Event.hpp"
#include "Input/GlfwInputMapping.hpp"
#include "Input/Input.hpp"

namespace orc {

namespace {

constexpr int OPENGL_MAJOR_VERSION = 4;
constexpr int OPENGL_MINOR_VERSION = 6;

void glfwErrorCallback(int code, const char* description) noexcept
{
	ORC_FATAL("GLFW error occurred...\n\tCode: {}\n\tDescription: {}", code, description);
}

}

Window::Window(const VideoSettings& videoSettings, Input& input)
	: m_videoSettings(videoSettings),
	  m_input(input)
{
	ORC_LOG_INFO("Initializing window.");
	ORC_LOG_INFO("Window:\n\tTitle: {}\n\tResolution: {}x{}\n\tVSync: {}", m_videoSettings.title, m_videoSettings.width, m_videoSettings.height, m_videoSettings.vsync ? "Enabled" : "Disabled");

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
	m_input.beginFrame();
	glfwPollEvents();
	glfwSwapBuffers(m_glfwWindow);
}

void Window::setTitle(std::string_view title)
{
	m_videoSettings.title = title;
	glfwSetWindowTitle(m_glfwWindow, m_videoSettings.title.c_str());
}

void Window::setEventCallback(EventCallback eventCallback)
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

void Window::onWindowResize(uint32_t width, uint32_t height)
{
	const bool windowChanged = (m_videoSettings.width != width || m_videoSettings.height != height);
	m_videoSettings.width = width;
	m_videoSettings.height = height;

	const uint32_t prevFbWidth = m_framebufferWidth;
	const uint32_t prevFbHeight = m_framebufferHeight;
	updateFramebufferDimensions();
	const bool fbChanged = (m_framebufferWidth != prevFbWidth || m_framebufferHeight != prevFbHeight);

	if ((windowChanged || fbChanged) && m_videoSettings.eventCallback)
	{
		m_videoSettings.eventCallback(Event::WindowResized);
	}
}

void Window::onFramebufferResize(uint32_t width, uint32_t height)
{
	if (m_framebufferWidth == width && m_framebufferHeight == height)
	{
		return;
	}

	m_framebufferWidth = width;
	m_framebufferHeight = height;

	int winWidth = 0;
	int winHeight = 0;
	glfwGetWindowSize(m_glfwWindow, &winWidth, &winHeight);
	m_videoSettings.width = static_cast<uint32_t>(winWidth);
	m_videoSettings.height = static_cast<uint32_t>(winHeight);

	if (m_videoSettings.eventCallback)
	{
		m_videoSettings.eventCallback(Event::WindowResized);
	}
}

void Window::initGLAD()
{
	ORC_LOG_INFO("Initializing GLFW.");
	ORC_FATAL_CHECK(glfwInit(), "Failed to initialize GLFW.");

	glfwSetErrorCallback(&glfwErrorCallback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_MAJOR_VERSION);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_MINOR_VERSION);

	m_glfwWindow = glfwCreateWindow(static_cast<int>(m_videoSettings.width), static_cast<int>(m_videoSettings.height), m_videoSettings.title.c_str(), nullptr, nullptr);
	ORC_FATAL_CHECK(m_glfwWindow != nullptr, "Failed to create GLFW window.");

	glfwMakeContextCurrent(m_glfwWindow);
	updateFramebufferDimensions();

	double cursorX = 0.0;
	double cursorY = 0.0;
	glfwGetCursorPos(m_glfwWindow, &cursorX, &cursorY);
	m_input.setMousePosition(static_cast<float>(cursorX), static_cast<float>(cursorY));

	glfwSetWindowUserPointer(m_glfwWindow, this);

	ORC_LOG_INFO("Initializing GLAD.");
	ORC_FATAL_CHECK(gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)), "Failed to initialize GLAD.");

	ORC_LOG_INFO("Graphics:\n\tOpenGL version: {}.{}\n\tVendor: {}\n\tRenderer: {}", OPENGL_MAJOR_VERSION, OPENGL_MINOR_VERSION, reinterpret_cast<const char*>(glGetString(GL_VENDOR)), reinterpret_cast<const char*>(glGetString(GL_RENDERER)));

	setCallbacks();
	setVsync(m_videoSettings.vsync);
}

void Window::setCallbacks()
{
	glfwSetWindowSizeCallback(m_glfwWindow,
							  [](GLFWwindow* window, int width, int height) noexcept {
								  Window& self = *reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
								  self.onWindowResize(static_cast<uint32_t>(width), static_cast<uint32_t>(height));
							  });

	glfwSetFramebufferSizeCallback(m_glfwWindow,
								   [](GLFWwindow* window, int width, int height) noexcept {
									   Window& self = *reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
									   self.onFramebufferResize(static_cast<uint32_t>(width), static_cast<uint32_t>(height));
								   });

	glfwSetWindowCloseCallback(m_glfwWindow,
							   [](GLFWwindow* window) noexcept {
								   Window& self = *reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
								   if (self.m_videoSettings.eventCallback)
								   {
									   self.m_videoSettings.eventCallback(Event::WindowClosed);
								   }
							   });

	glfwSetKeyCallback(m_glfwWindow,
					   [](GLFWwindow* window, int key, [[maybe_unused]]int scancode, int action, int mods) noexcept {
						   Window& self = *reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
						   const Keyboard::Key orcKey = glfw::glfwKeyToOrcKey(key, mods);
						   const Keyboard::SpecialKeys specialKeys = glfw::glfwKeyModsToOrcSpecialKeys(mods);

						   if (action == GLFW_PRESS)
						   {
							   self.m_input.onKeyPressed(orcKey, specialKeys);
						   }
						   else if (action == GLFW_RELEASE)
						   {
							   self.m_input.onKeyReleased(orcKey, specialKeys);
						   }
					   });

	glfwSetMouseButtonCallback(m_glfwWindow,
							   [](GLFWwindow* window, int button, int action, [[maybe_unused]]int mods) noexcept {
								   Window& self = *reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
								   const Mouse::Button orcButton = glfw::glfwButtonToOrcButton(button);

								   if (action == GLFW_PRESS)
								   {
									   self.m_input.onMouseButtonPressed(orcButton);
								   }
								   else if (action == GLFW_RELEASE)
								   {
									   self.m_input.onMouseButtonReleased(orcButton);
								   }
							   });

	glfwSetCursorPosCallback(m_glfwWindow,
							 [](GLFWwindow* window, double x, double y) noexcept {
								 Window& self = *reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
								 self.m_input.onMouseMoved(static_cast<float>(x), static_cast<float>(y));
							 });

	glfwSetScrollCallback(m_glfwWindow,
						  [](GLFWwindow* window, double xDelta, double yDelta) noexcept {
							  Window& self = *reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
							  self.m_input.onMouseScrolled(static_cast<float>(xDelta), static_cast<float>(yDelta));
						  });
}

}
