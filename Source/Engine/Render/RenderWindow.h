
#ifndef renderwindow_h
#define renderwindow_h

#include <eternity.h>
#include <Core/Window.h>

/**
 * Basic platform-specific window
 */
class GIGA_API RenderWindow : public Window {
public:
	RenderWindow() = default;
	~RenderWindow() = default;

	GIGA_CLASS_NAME("RenderWindow");

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
	// Frame width and height (different for retina)
	int m_framebufferWidth, m_framebufferHeight;
};

#endif
