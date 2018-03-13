
#include <Render/RenderWindow.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

bool RenderWindow::m_initialized = false;

void RenderWindow::Create(std::string name, int width, int height, bool fullscreen) {
	// If we do not have a window yet, initialize it
	if (m_initialized == false) {
		glfwInit();

		// Specify the minimum OpenGL version
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_SAMPLES, 4);
	}

	// Save properties
	m_width = width;
	m_height = height;
	m_name = name;
	m_fullscreen = fullscreen;

	// Get our primary monitor
	GLFWmonitor* primary = glfwGetPrimaryMonitor();
	const GLFWvidmode * mode = glfwGetVideoMode(primary);
	unsigned int w = mode->width;
	unsigned int h = mode->height;

	// Create our window
	m_window = glfwCreateWindow(fullscreen ? w : width, fullscreen ? h : height, name.c_str(), fullscreen ? primary : NULL, NULL);
	if (m_window == NULL) {
		glfwTerminate();
		//Message::Broadcast(Error::MSGTYPE_ERROR, new Error(Error::ERROR_FATAL, "Unable to create window."));
		return;
	}

	// Make our OpenGL context current
	glfwMakeContextCurrent((GLFWwindow*)m_window);

	// Get our actual framebuffer size
	glfwGetFramebufferSize((GLFWwindow*)m_window, &m_framebufferWidth, &m_framebufferHeight);

	glViewport(0, 0, m_framebufferWidth, m_framebufferHeight);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	if (RenderWindow::m_initialized == false) {
#ifdef WIN32
		glewExperimental = true;
		GLenum err = glewInit();
		if (GLEW_OK != err) {
			//Message::Broadcast(Error::MSGTYPE_ERROR, new Error(Error::ERROR_FATAL, "Unable to initialize GLEW."));
			return;
		}
#endif
	}
}

bool RenderWindow::IsClosing() {
	return(glfwWindowShouldClose((GLFWwindow*)m_window));
}

void RenderWindow::ProcessEvents() {
	glfwPollEvents();
}

void RenderWindow::SwapBuffer() {
	glfwSwapBuffers((GLFWwindow*)m_window);
}

void RenderWindow::GetFramebufferSize(int& width, int& height) {
    width = m_framebufferWidth;
    height = m_framebufferHeight;
}
