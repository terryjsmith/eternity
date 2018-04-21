#ifndef keyboard_hpp
#define keyboard_hpp

#include <eternity.h>
#include <Core/Message.h>
#include <IO/InputDevice.h>

enum {
    KEY_PRESS = 1,
    KEY_RELEASE = 2
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
};

#endif
