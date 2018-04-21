
#include <IO/InputDevice.h>

bool InputDevice::GetButtonState(int button) {
    std::map<int, bool>::iterator i = m_buttons.find(button);
    if (i == m_buttons.end()) {
        return(false);
    }
    
    return(m_buttons[button]);
}
