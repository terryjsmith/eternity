
#include <IO/Keyboard.h>
#include <IO/InputSystem.h>
#include <GLFW/glfw3.h>
#include <Core/Application.h>

Keyboard::Keyboard() {
    m_type = INPUTDEVICE_KEYBOARD;
}

// GLFW callback functions
void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // Get our input system
    InputSystem* ioSystem = GetSystem<InputSystem>();
    
    // Find our keyboard device
    Keyboard* keyboard = dynamic_cast<Keyboard*>(ioSystem->GetInputDevice(INPUTDEVICE_KEYBOARD));
    
    // Dispatch event
    keyboard->ProcessAction(key, action);
}

void Keyboard::Initialize() {
    // Get our window
    Window* window = Application::GetInstance()->GetPrimaryWindow();
    GLFWwindow* glfwWindow = (GLFWwindow*)window->GetWindowHandle();
    
    glfwSetKeyCallback(glfwWindow, KeyboardCallback);
    
    /* Get our scripting system and register globals
    ScriptingSystem* scriptingSystem = GetSystem<ScriptingSystem>();
    
    // Keyboard
    ScriptableObjectType* keyboardType = new ScriptableObjectType("Keyboard");
    keyboardType->AddObjectFunction("GetButtonState", &Keyboard::GetButtonState);
    
    scriptingSystem->Lock(scriptingSystem);
    scriptingSystem->RegisterScriptableObjectType<Keyboard>(keyboardType);
    
    // Keyboard event data
    ScriptableObjectType* keyboardEventType = new ScriptableObjectType("KeyboardEventData");
    keyboardEventType->AddObjectVariable("key", &KeyboardEventData::GetKey, 0);
    
    scriptingSystem->RegisterScriptableObjectType<KeyboardEventData>(keyboardEventType);
    
    // Keyboard global variables/bindings
    scriptingSystem->RegisterGlobal("KEY_CODE_A", new Variant(GLFW_KEY_A));
    scriptingSystem->RegisterGlobal("KEY_CODE_B", new Variant(GLFW_KEY_B));
    scriptingSystem->RegisterGlobal("KEY_CODE_C", new Variant(GLFW_KEY_C));
    scriptingSystem->RegisterGlobal("KEY_CODE_D", new Variant(GLFW_KEY_D));
    scriptingSystem->RegisterGlobal("KEY_CODE_E", new Variant(GLFW_KEY_E));
    scriptingSystem->RegisterGlobal("KEY_CODE_F", new Variant(GLFW_KEY_F));
    scriptingSystem->RegisterGlobal("KEY_CODE_G", new Variant(GLFW_KEY_G));
    scriptingSystem->RegisterGlobal("KEY_CODE_H", new Variant(GLFW_KEY_H));
    scriptingSystem->RegisterGlobal("KEY_CODE_I", new Variant(GLFW_KEY_I));
    scriptingSystem->RegisterGlobal("KEY_CODE_J", new Variant(GLFW_KEY_J));
    scriptingSystem->RegisterGlobal("KEY_CODE_K", new Variant(GLFW_KEY_K));
    scriptingSystem->RegisterGlobal("KEY_CODE_L", new Variant(GLFW_KEY_L));
    scriptingSystem->RegisterGlobal("KEY_CODE_M", new Variant(GLFW_KEY_M));
    scriptingSystem->RegisterGlobal("KEY_CODE_N", new Variant(GLFW_KEY_N));
    scriptingSystem->RegisterGlobal("KEY_CODE_O", new Variant(GLFW_KEY_O));
    scriptingSystem->RegisterGlobal("KEY_CODE_P", new Variant(GLFW_KEY_P));
    scriptingSystem->RegisterGlobal("KEY_CODE_Q", new Variant(GLFW_KEY_Q));
    scriptingSystem->RegisterGlobal("KEY_CODE_R", new Variant(GLFW_KEY_R));
    scriptingSystem->RegisterGlobal("KEY_CODE_S", new Variant(GLFW_KEY_S));
    scriptingSystem->RegisterGlobal("KEY_CODE_T", new Variant(GLFW_KEY_T));
    scriptingSystem->RegisterGlobal("KEY_CODE_U", new Variant(GLFW_KEY_U));
    scriptingSystem->RegisterGlobal("KEY_CODE_V", new Variant(GLFW_KEY_V));
    scriptingSystem->RegisterGlobal("KEY_CODE_W", new Variant(GLFW_KEY_W));
    scriptingSystem->RegisterGlobal("KEY_CODE_X", new Variant(GLFW_KEY_X));
    scriptingSystem->RegisterGlobal("KEY_CODE_Y", new Variant(GLFW_KEY_Y));
    scriptingSystem->RegisterGlobal("KEY_CODE_Z", new Variant(GLFW_KEY_Z));
    scriptingSystem->RegisterGlobal("KEY_CODE_0", new Variant(GLFW_KEY_0));
    scriptingSystem->RegisterGlobal("KEY_CODE_1", new Variant(GLFW_KEY_1));
    scriptingSystem->RegisterGlobal("KEY_CODE_2", new Variant(GLFW_KEY_2));
    scriptingSystem->RegisterGlobal("KEY_CODE_3", new Variant(GLFW_KEY_3));
    scriptingSystem->RegisterGlobal("KEY_CODE_4", new Variant(GLFW_KEY_4));
    scriptingSystem->RegisterGlobal("KEY_CODE_5", new Variant(GLFW_KEY_5));
    scriptingSystem->RegisterGlobal("KEY_CODE_6", new Variant(GLFW_KEY_6));
    scriptingSystem->RegisterGlobal("KEY_CODE_7", new Variant(GLFW_KEY_7));
    scriptingSystem->RegisterGlobal("KEY_CODE_8", new Variant(GLFW_KEY_8));
    scriptingSystem->RegisterGlobal("KEY_CODE_9", new Variant(GLFW_KEY_9));
    scriptingSystem->RegisterGlobal("KEY_CODE_UP", new Variant(GLFW_KEY_UP));
    scriptingSystem->RegisterGlobal("KEY_CODE_DOWN", new Variant(GLFW_KEY_DOWN));
    scriptingSystem->RegisterGlobal("KEY_CODE_LEFT", new Variant(GLFW_KEY_LEFT));
    scriptingSystem->RegisterGlobal("KEY_CODE_RIGHT", new Variant(GLFW_KEY_RIGHT));
    scriptingSystem->RegisterGlobal("KEY_CODE_FORWARD_SLASH", new Variant(GLFW_KEY_SLASH));
    scriptingSystem->RegisterGlobal("KEY_CODE_EQUAL", new Variant(GLFW_KEY_EQUAL));
    scriptingSystem->RegisterGlobal("KEY_CODE_PERIOD", new Variant(GLFW_KEY_PERIOD));
    scriptingSystem->RegisterGlobal("KEY_CODE_COMMA", new Variant(GLFW_KEY_COMMA));
    scriptingSystem->RegisterGlobal("KEY_CODE_SEMICOLON", new Variant(GLFW_KEY_SEMICOLON));
    scriptingSystem->RegisterGlobal("KEY_CODE_BACKSLASH", new Variant(GLFW_KEY_BACKSLASH));
    scriptingSystem->RegisterGlobal("KEY_CODE_LEFT_SHIFT", new Variant(GLFW_KEY_LEFT_SHIFT));
    scriptingSystem->RegisterGlobal("KEY_CODE_RIGHT_SHIFT", new Variant(GLFW_KEY_RIGHT_SHIFT));
    scriptingSystem->RegisterGlobal("KEY_CODE_DELETE", new Variant(GLFW_KEY_DELETE));
    scriptingSystem->RegisterGlobal("KEY_CODE_ENTER", new Variant(GLFW_KEY_ENTER));
    scriptingSystem->RegisterGlobal("KEY_CODE_TAB", new Variant(GLFW_KEY_TAB));
    scriptingSystem->RegisterGlobal("KEY_CODE_LEFT_CONTROL", new Variant(GLFW_KEY_LEFT_CONTROL));
    scriptingSystem->RegisterGlobal("KEY_CODE_RIGHT_CONTROL", new Variant(GLFW_KEY_RIGHT_CONTROL));
    scriptingSystem->RegisterGlobal("KEY_CODE_LEFT_ALT", new Variant(GLFW_KEY_LEFT_ALT));
    scriptingSystem->RegisterGlobal("KEY_CODE_RIGHT_ALT", new Variant(GLFW_KEY_RIGHT_ALT));
    scriptingSystem->RegisterGlobal("KEY_CODE_HYPHEN", new Variant(GLFW_KEY_MINUS));
    scriptingSystem->RegisterGlobal("KEY_CODE_SPACE", new Variant(GLFW_KEY_SPACE));
    scriptingSystem->RegisterGlobal("KEY_CODE_F1", new Variant(GLFW_KEY_F1));
    scriptingSystem->RegisterGlobal("KEY_CODE_F2", new Variant(GLFW_KEY_F2));
    scriptingSystem->RegisterGlobal("KEY_CODE_F3", new Variant(GLFW_KEY_F3));
    scriptingSystem->RegisterGlobal("KEY_CODE_F4", new Variant(GLFW_KEY_F4));
    scriptingSystem->RegisterGlobal("KEY_CODE_F5", new Variant(GLFW_KEY_F5));
    scriptingSystem->RegisterGlobal("KEY_CODE_F6", new Variant(GLFW_KEY_F6));
    scriptingSystem->RegisterGlobal("KEY_CODE_F7", new Variant(GLFW_KEY_F7));
    scriptingSystem->RegisterGlobal("KEY_CODE_F8", new Variant(GLFW_KEY_F8));
    scriptingSystem->RegisterGlobal("KEY_CODE_F9", new Variant(GLFW_KEY_F9));
    scriptingSystem->RegisterGlobal("KEY_CODE_PAGE_UP", new Variant(GLFW_KEY_PAGE_UP));
    scriptingSystem->RegisterGlobal("KEY_CODE_PAGE_DOWN", new Variant(GLFW_KEY_PAGE_DOWN));
    scriptingSystem->RegisterGlobal("KEY_CODE_ESCAPE", new Variant(GLFW_KEY_ESCAPE));
    scriptingSystem->RegisterGlobal("KEY_CODE_APOSTROPHE", new Variant(GLFW_KEY_APOSTROPHE));
    scriptingSystem->RegisterGlobal("KEY_CODE_PRINT_SCREEN", new Variant(GLFW_KEY_PRINT_SCREEN));
    
    scriptingSystem->Unlock();*/
}

void Keyboard::ProcessAction(int key, int action) {
    if (action == GLFW_PRESS) {
        Message::Broadcast(new KeyboardMessage(key, KEY_PRESS));
    }
    
    if (action == GLFW_RELEASE) {
        Message::Broadcast(new KeyboardMessage(key, KEY_RELEASE));
    }
}
