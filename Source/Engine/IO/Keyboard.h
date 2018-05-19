#ifndef keyboard_hpp
#define keyboard_hpp

#include <eternity.h>
#include <Core/Message.h>
#include <IO/InputDevice.h>

enum {
    KEY_PRESS = 1,
    KEY_RELEASE = 2
};

enum Keys {
	KEY_0 = 1,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_A,
	KEY_B,
	KEY_C, 
	KEY_D,
	KEY_E,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_N,
	KEY_O,
	KEY_P,
	KEY_Q,
	KEY_R,
	KEY_S,
	KEY_T,
	KEY_U,
	KEY_V,
	KEY_W,
	KEY_X,
	KEY_Y,
	KEY_Z,
	KEY_TAB,
	KEY_LEFT_SHIFT,
	KEY_RIGHT_SHIFT,
	KEY_LEFT_CTRL,
	KEY_RIGHT_CTRL,
	KEY_LEFT_ALT,
	KEY_RIGHT_ALT,
	KEY_SPACE,
	KEY_ESCAPE,
	KEY_BACKSPACE,
	KEY_ENTER,
	KEY_EQUALS,
	KEY_MINUS,
	KEY_LEFT_SQUARE_BRACKET,
	KEY_RIGHT_SQUARE_BRACKET,
	KEY_BACK_SLASH,
	KEY_FORWARD_SLASH,
	KEY_COMMA,
	KEY_PERIOD,
	KEY_INSERT,
	KEY_DELETE,
	KEY_HOME,
	KEY_END,
	KEY_PAGE_UP,
	KEY_PAGE_DOWN,
	KEY_PRINT_SCREEN,
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_NUMPAD_PLUS,
	KEY_NUMPAD_0,
	KEY_NUMPAD_1,
	KEY_NUMPAD_2,
	KEY_NUMPAD_3,
	KEY_NUMPAD_4,
	KEY_NUMPAD_5,
	KEY_NUMPAD_6,
	KEY_NUMPAD_7,
	KEY_NUMPAD_8, 
	KEY_NUMPAD_9,
	KEY_FUNC_F1,
	KEY_FUNC_F2,
	KEY_FUNC_F3,
	KEY_FUNC_F4,
	KEY_FUNC_F5,
	KEY_FUNC_F6,
	KEY_FUNC_F7,
	KEY_FUNC_F8,
	KEY_FUNC_F9,
	KEY_FUNC_F10,
	KEY_FUNC_F11,
	KEY_FUNC_F12,
	KEY_LAST
};

/**
 * Keyboard event data
 */
GIGA_CLASS()
class GIGA_API KeyboardMessage : public Message {
public:
    GIGA_CLASS_NAME("KeyboardMessage");
    
    KeyboardMessage() = default;
    KeyboardMessage(int key, int action) : m_key(key), m_action(action) { }
    
    GIGA_FUNCTION() int GetKey() { return m_key; }
    GIGA_FUNCTION() int GetAction() { return m_action; }
    
protected:
    // Key that was pressed or released
    int m_key;
    int m_action;
};

/**
 * Keyboard input device
 */
GIGA_CLASS()
class GIGA_API Keyboard : public InputDevice {
public:
    Keyboard();
    ~Keyboard() = default;
    
    GIGA_CLASS_NAME("Keyboard")
    
    /**
     * Initialize our input device
     */
    void Initialize();
    
    /**
     * Handle a keyboard action (specific callback for our platform library)
     */
    void ProcessAction(int key, int action);

protected:
	// Key mappings
	int m_keyMappings[KEY_LAST];
};

#endif
