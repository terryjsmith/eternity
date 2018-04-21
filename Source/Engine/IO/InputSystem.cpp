
#include <IO/InputSystem.h>

InputSystem::~InputSystem() {
    for(size_t i = 0; i < m_mappings.size(); i++) {
        delete m_mappings[i];
    }
}

void InputSystem::Initialize() {
    
}

void InputSystem::Update(float delta) {
    for(size_t i = 0; i < m_devices.size(); i++) {
        m_devices[i]->Update();
    }
}

void InputSystem::RegisterInputDevice(InputDevice* device) {
    int type = device->GetType();
    for(size_t i = 0; i < m_devices.size(); i++) {
        if(m_devices[i]->GetType() == type) {
            GIGA_ASSERT(false, "There is already an input device of this type registered.");
        }
    }
    
    m_devices.push_back(device);
}

InputDevice* InputSystem::GetInputDevice(int type) {
    for(size_t i = 0; i < m_devices.size(); i++) {
        if(m_devices[i]->GetType() == type) {
            return(m_devices[i]);
        }
    }
    
    return(0);
}

void InputSystem::RegisterInputMapping(InputDevice* device, int button, std::string command) {
    // Do we already have something generating this command? If so, re-map
    for(size_t i = 0; i < m_mappings.size(); i++) {
        if(m_mappings[i]->command == command) {
            m_mappings[i]->device = device;
            m_mappings[i]->button = button;
            return;
        }
    }
    
    // Otherwise create a new mapping
    InputMapping* mapping = new InputMapping();
    mapping->device = device;
    mapping->button = button;
    mapping->command = command;
    
    m_mappings.push_back(mapping);
}

std::string InputSystem::FindInputMapping(InputDevice* device, int button) {
    std::string command;
    for(size_t i = 0; i < m_mappings.size(); i++) {
        if(m_mappings[i]->device == device && m_mappings[i]->button == button) {
            command = m_mappings[i]->command;
            break;
        }
    }
    
    return(command);
}
