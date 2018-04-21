#ifndef inputdevice_hpp
#define inputdevice_hpp

#include <eternity.h>
#include <Core/GigaObject.h>

enum {
    INPUTDEVICE_KEYBOARD = 1,
    INPUTDEVICE_MOUSE,
    INPUTDEVICE_JOYSTICK
};

/**
 * Generic input device base class (to be initialized by platform)
 */
class GIGA_API InputDevice : public GigaObject {
public:
    ~InputDevice() = default;
    
    /**
     * Called once on device registration (should handle any system registrations)
     */
    virtual void Initialize() { }
    
    /**
     * Virtual update function (to poll device)
     */
    virtual void Update() { }
    
    /**
     * Get the state of a particular button
     */
    GIGA_FUNCTION() bool GetButtonState(int button);
    
    /**
     * Get/set device type
     */
    void SetType(int type) { m_type = type; }
    int GetType() { return m_type; }
    
protected:
    // Protected constructor so it can't be created directly
    InputDevice() = default;
    
protected:
    // Button mappings (and their pressed state)
    std::map<int, bool> m_buttons;
    
    // Type of device
    int m_type;
};

#endif
