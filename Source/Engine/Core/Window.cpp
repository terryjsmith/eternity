
#include <Core/Window.h>

bool Window::m_initialized = false;

Window::Window() {
	m_width = 0;
	m_height = 0;
	m_fullscreen = false;
	m_window = 0;
}