
#ifndef renderwindow_h
#define renderwindow_h

#include <eternity.h>

/**
 * Basic platform-specific window
 */
class GIGA_API RenderWindow {
public:
	RenderWindow() = default;
	~RenderWindow() = default;

	void Create(std::string name, int width, int height, bool fullscreen);

	/**
	* Whether the window is closed/closing or not
	*/
	bool IsClosing();

	/**
	* Platform-specific event loop
	*/
	void ProcessEvents();

	/**
	* Swap window buffers
	*/
	void SwapBuffer();
    
    /**
     * Get framebuffer width/height
     */
    void GetFramebufferSize(int& width, int& height);

protected:
	// Window width and height
	int m_width, m_height;

	// Frame width and height (different for retina)
	int m_framebufferWidth, m_framebufferHeight;

	bool m_fullscreen;
	std::string m_name;

	// Whether GLFW and other window libraries are initialized yet
	static bool m_initialized;

	// Internal window pointer
	void *m_window;
};

#endif
