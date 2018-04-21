#ifndef inputsystem_hpp
#define inputsystem_hpp

#include <eternity.h>
#include <Core/System.h>
#include <IO/Command.h>
#include <IO/InputDevice.h>

GIGA_CLASS(Singleton)
class GIGA_API InputSystem : public System {
public:
    InputSystem() = default;
    ~InputSystem();
    
    GIGA_CLASS_NAME("InputSystem");
    
    /**
     * Initialize (scripting interfaces)
     */
    void Initialize();
    
    /**
     * Update input devices (polling mostly)
     */
    void Update(float delta);
    
    /**
     * Register a new input device (mouse, keyboard, joystick)
     */
    void RegisterInputDevice(InputDevice* device);
    
    /**
     * Get a device by device type
     */
    GIGA_FUNCTION() InputDevice* GetInputDevice(int type);
    
    /**
     * Set a binding for a command event to be created
     */
    GIGA_FUNCTION() void RegisterInputMapping(InputDevice* device, int button, std::string command);
    
    /**
     * Find an input mapping
     */
    GIGA_FUNCTION() std::string FindInputMapping(InputDevice* device, int button);
    
private:
    struct InputMapping {
        int button;
        InputDevice* device;
        std::string command;
    };
    
protected:
    // List of registered input devices (max one per type)
    std::vector<InputDevice*> m_devices;
    
    // List of registered input mappings
    std::vector<InputMapping*> m_mappings;
    
    // History of commands
    std::vector<Command*> m_commands;
};

#endif
