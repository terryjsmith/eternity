
#ifndef window_h
#define window_h

#include <eternity.h>
#include <Core/GigaObject.h>

class GIGA_API Window : public GigaObject {
public:
	~Window() = default;

	virtual void Create(std::string name, int width, int height, bool fullscreen) { }

	/**
	* Whether the window is closed/closing or not
	*/
	virtual bool IsClosing() { return(false); }

	/**
	* Platform-specific event loop
	*/
	virtual void ProcessEvents() { }

	/**
	* Swap window buffers
	*/
	virtual void SwapBuffer() { }

	/**
	* Get framebuffer width/height
	*/
	virtual void GetFramebufferSize(int& width, int& height) { }

	/**
	 * Getters
	 */
	int GetWidth() { return m_width; }
	int GetHeight() { return m_height; }

	std::string GetName() { return m_name; }
	bool IsFullscreen() { return m_fullscreen; }

	void* GetWindowHandle() { return m_window; }

protected:
	// No direct creation
	Window();

protected:
	// Window width and height
	int m_width, m_height;

	bool m_fullscreen;
	std::string m_name;

	// Whether GLFW and other window libraries are initialized yet
	static bool m_initialized;

	// Internal window pointer
	void *m_window;
};

#endif