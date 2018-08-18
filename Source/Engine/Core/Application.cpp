
#include <Core/Application.h>
#include <IO/ResourceSystem.h>
#include <Core/MessageSystem.h>
#include <Core/MetaSystem.h>
#include <IO/InputSystem.h>
#include <Render/Shader.h>
#include <Core/Error.h>
#include <IO/Keyboard.h>
#include <IO/Command.h>
#include <Core/TransformComponent.h>
#include <Scripting/Script.h>
#include <Scripting/ScriptingSystem.h>
#include <Render/MeshComponent.h>
#include <Render/CameraComponent.h>
#include <Render/SpotLightComponent.h>
#include <Render/PointLightComponent.h>
#include <Physics/PhysicsCollision.h>
#include <Physics/RigidBodyComponent.h>
#include <Core/DataRecordType.h>
#include <Network/NetworkSystem.h>
#include <Network/Messages/NetworkMessageTypes.h>
#include <Network/Messages/EchoRequestMessage.h>
#include <Network/Messages/EchoResponseMessage.h>
#include <Network/Messages/AckMessage.h>
#include <Network/Messages/EntitySnapshotMessage.h>
#include <Network/Messages/CommandMessage.h>
#include <Network/Messages/ResendPartialMessage.h>
#include <Network/Messages/HelloMessage.h>
#include <Core/Entity.h>

Application* Application::m_instance = 0;

Application::Application() {
	m_primaryWindow = 0;
    m_world = 0;
}

void Application::Initialize() {
	// Get systems
    InputSystem* inputSystem = GetSystem<InputSystem>();
    ScriptingSystem* scriptingSystem = GetSystem<ScriptingSystem>();
    MessageSystem* messageSystem = GetSystem<MessageSystem>();
    ResourceSystem* resourceSystem = GetSystem<ResourceSystem>();
    MetaSystem* metaSystem = GetSystem<MetaSystem>();
    NetworkSystem* networkSystem = GetSystem<NetworkSystem>();
    
	// Register messages
	messageSystem->RegisterMessageType<Error>("Error");
	messageSystem->RegisterMessageType<KeyboardMessage>("KeyboardMessage");
	messageSystem->RegisterMessageType<Command>("Command");
    messageSystem->RegisterMessageType<PhysicsCollision>("PhysicsCollision");
    messageSystem->RegisterMessageType<Command>("Command");

	// Register resource types
	resourceSystem->RegisterResourceType<Shader>("Shader");
    resourceSystem->RegisterResourceType<Script>("Script");
    resourceSystem->RegisterResourceType<Mesh>("Mesh");

	// Register singletons and callback functions
	metaSystem->RegisterSingleton("ResourceSystem", resourceSystem);
	metaSystem->RegisterSingleton("InputSystem", inputSystem);
    metaSystem->RegisterSingleton("ScriptingSystem", scriptingSystem);
    metaSystem->RegisterSingleton("NetworkSystem", networkSystem);
    
    // Message types
    networkSystem->RegisterMessageType<EchoRequestMessage>(NetworkMessages::idEchoRequestMessage);
    networkSystem->RegisterMessageType<EchoResponseMessage>(NetworkMessages::idEchoResponseMessage);
    networkSystem->RegisterMessageType<AckMessage>(NetworkMessages::idAckMessage);
    networkSystem->RegisterMessageType<EntitySnapshotMessage>(NetworkMessages::idEntitySnapshotMessage);
    networkSystem->RegisterMessageType<CommandMessage>(NetworkMessages::idCommandMessage);
    networkSystem->RegisterMessageType<ResendPartialMessage>(NetworkMessages::idResendPartialMessage);
    networkSystem->RegisterMessageType<HelloMessage>(NetworkMessages::idHelloMessage);
    
    // Component types
    Component::RegisterComponentType<TransformComponent>("TransformComponent", 5);
    Component::RegisterComponentType<MeshComponent>("MeshComponent", 10);
    Component::RegisterComponentType<ScriptComponent>("ScriptComponent", 15);
    Component::RegisterComponentType<CameraComponent>("CameraComponent", 20);
    Component::RegisterComponentType<SpotLightComponent>("SpotLightComponent", 25);
    Component::RegisterComponentType<PointLightComponent>("PointLightComponent", 30);
    // TODO: Cascaded Directional Light
    Component::RegisterComponentType<RigidBodyComponent>("RigidBodyComponent", 40);
    
    // Data record types
    DataRecordType* entityType = new DataRecordType();
    entityType->SetPrimaryKey("Entity_id");
    entityType->AddKey("id", Variant::VAR_INT32);
    entityType->AddKey("name", Variant::VAR_STRING);
    entityType->AddKey("active", Variant::VAR_BOOL);
    DataRecordType::Register<Entity>("Entity", entityType);
    
    DataRecordType* transformComponentType = new DataRecordType();
    transformComponentType->SetPrimaryKey("TransformComponent_id");
    transformComponentType->AddKey("entityID", Variant::VAR_INT32);
    transformComponentType->AddKey("position", Variant::VAR_VECTOR3);
    transformComponentType->AddKey("rotation", Variant::VAR_QUATERNION);
    transformComponentType->AddKey("scale", Variant::VAR_VECTOR3);
    DataRecordType::Register<TransformComponent>("TransformComponent", transformComponentType);
    
    DataRecordType* meshComponentType = new DataRecordType();
    meshComponentType->SetPrimaryKey("MeshComponent_id");
    meshComponentType->AddKey("entityID", Variant::VAR_INT32);
    meshComponentType->AddKey("mesh", Variant::VAR_STRING);
    meshComponentType->AddKey("position", Variant::VAR_VECTOR3);
    meshComponentType->AddKey("rotation", Variant::VAR_QUATERNION);
    meshComponentType->AddKey("scale", Variant::VAR_VECTOR3);
    DataRecordType::Register<MeshComponent>("MeshComponent", meshComponentType);
    
    // Device types
    scriptingSystem->SetGlobal("INPUTDEVICE_MOUSE", new Variant(INPUTDEVICE_MOUSE));
    scriptingSystem->SetGlobal("INPUTDEVICE_KEYBOARD", new Variant(INPUTDEVICE_KEYBOARD));
    
    // World global
    scriptingSystem->SetGlobal("World", new Variant(m_world));
    
    // Keyboard keys
    scriptingSystem->SetGlobal("KEY_0", new Variant(KEY_0));
    scriptingSystem->SetGlobal("KEY_1", new Variant(KEY_1));
    scriptingSystem->SetGlobal("KEY_2", new Variant(KEY_2));
    scriptingSystem->SetGlobal("KEY_3", new Variant(KEY_3));
    scriptingSystem->SetGlobal("KEY_4", new Variant(KEY_4));
    scriptingSystem->SetGlobal("KEY_5", new Variant(KEY_5));
    scriptingSystem->SetGlobal("KEY_6", new Variant(KEY_6));
    scriptingSystem->SetGlobal("KEY_7", new Variant(KEY_7));
    scriptingSystem->SetGlobal("KEY_8", new Variant(KEY_8));
    scriptingSystem->SetGlobal("KEY_9", new Variant(KEY_9));
    scriptingSystem->SetGlobal("KEY_A", new Variant(KEY_A));
    scriptingSystem->SetGlobal("KEY_B", new Variant(KEY_B));
    scriptingSystem->SetGlobal("KEY_C", new Variant(KEY_C));
    scriptingSystem->SetGlobal("KEY_D", new Variant(KEY_D));
    scriptingSystem->SetGlobal("KEY_E", new Variant(KEY_E));
    scriptingSystem->SetGlobal("KEY_F", new Variant(KEY_F));
    scriptingSystem->SetGlobal("KEY_G", new Variant(KEY_G));
    scriptingSystem->SetGlobal("KEY_H", new Variant(KEY_H));
    scriptingSystem->SetGlobal("KEY_I", new Variant(KEY_I));
    scriptingSystem->SetGlobal("KEY_J", new Variant(KEY_J));
    scriptingSystem->SetGlobal("KEY_K", new Variant(KEY_K));
    scriptingSystem->SetGlobal("KEY_L", new Variant(KEY_L));
    scriptingSystem->SetGlobal("KEY_M", new Variant(KEY_M));
    scriptingSystem->SetGlobal("KEY_N", new Variant(KEY_N));
    scriptingSystem->SetGlobal("KEY_O", new Variant(KEY_O));
    scriptingSystem->SetGlobal("KEY_P", new Variant(KEY_P));
    scriptingSystem->SetGlobal("KEY_Q", new Variant(KEY_Q));
    scriptingSystem->SetGlobal("KEY_R", new Variant(KEY_R));
    scriptingSystem->SetGlobal("KEY_S", new Variant(KEY_S));
    scriptingSystem->SetGlobal("KEY_T", new Variant(KEY_T));
    scriptingSystem->SetGlobal("KEY_U", new Variant(KEY_U));
    scriptingSystem->SetGlobal("KEY_V", new Variant(KEY_V));
    scriptingSystem->SetGlobal("KEY_W", new Variant(KEY_W));
    scriptingSystem->SetGlobal("KEY_X", new Variant(KEY_X));
    scriptingSystem->SetGlobal("KEY_Y", new Variant(KEY_Y));
    scriptingSystem->SetGlobal("KEY_Z", new Variant(KEY_Z));
    scriptingSystem->SetGlobal("KEY_TAB", new Variant(KEY_TAB));
    scriptingSystem->SetGlobal("KEY_LEFT_SHIFT", new Variant(KEY_LEFT_SHIFT));
    scriptingSystem->SetGlobal("KEY_RIGHT_SHIFT", new Variant(KEY_RIGHT_SHIFT));
    scriptingSystem->SetGlobal("KEY_LEFT_CTRL", new Variant(KEY_LEFT_CTRL));
    scriptingSystem->SetGlobal("KEY_RIGHT_CTRL", new Variant(KEY_RIGHT_CTRL));
    scriptingSystem->SetGlobal("KEY_LEFT_ALT", new Variant(KEY_LEFT_ALT));
    scriptingSystem->SetGlobal("KEY_RIGHT_ALT", new Variant(KEY_RIGHT_ALT));
    scriptingSystem->SetGlobal("KEY_SPACE", new Variant(KEY_SPACE));
    scriptingSystem->SetGlobal("KEY_ESCAPE", new Variant(KEY_ESCAPE));
    scriptingSystem->SetGlobal("KEY_BACKSPACE", new Variant(KEY_BACKSPACE));
    scriptingSystem->SetGlobal("KEY_ENTER", new Variant(KEY_ENTER));
    scriptingSystem->SetGlobal("KEY_EQUALS", new Variant(KEY_EQUALS));
    scriptingSystem->SetGlobal("KEY_MINUS", new Variant(KEY_MINUS));
    scriptingSystem->SetGlobal("KEY_LEFT_SQUARE_BRACKET", new Variant(KEY_LEFT_SQUARE_BRACKET));
    scriptingSystem->SetGlobal("KEY_RIGHT_SQUARE_BRACKET", new Variant(KEY_RIGHT_SQUARE_BRACKET));
    scriptingSystem->SetGlobal("KEY_BACK_SLASH", new Variant(KEY_BACK_SLASH));
    scriptingSystem->SetGlobal("KEY_FORWARD_SLASH", new Variant(KEY_FORWARD_SLASH));
    scriptingSystem->SetGlobal("KEY_COMMA", new Variant(KEY_COMMA));
    scriptingSystem->SetGlobal("KEY_PERIOD", new Variant(KEY_PERIOD));
    scriptingSystem->SetGlobal("KEY_INSERT", new Variant(KEY_INSERT));
    scriptingSystem->SetGlobal("KEY_DELETE", new Variant(KEY_DELETE));
    scriptingSystem->SetGlobal("KEY_HOME", new Variant(KEY_HOME));
    scriptingSystem->SetGlobal("KEY_END", new Variant(KEY_END));
    scriptingSystem->SetGlobal("KEY_PAGE_UP", new Variant(KEY_PAGE_UP));
    scriptingSystem->SetGlobal("KEY_PAGE_DOWN", new Variant(KEY_PAGE_DOWN));
    scriptingSystem->SetGlobal("KEY_PRINT_SCREEN", new Variant(KEY_PRINT_SCREEN));
    scriptingSystem->SetGlobal("KEY_UP", new Variant(KEY_UP));
    scriptingSystem->SetGlobal("KEY_DOWN", new Variant(KEY_DOWN));
    scriptingSystem->SetGlobal("KEY_LEFT", new Variant(KEY_LEFT));
    scriptingSystem->SetGlobal("KEY_RIGHT", new Variant(KEY_RIGHT));
    scriptingSystem->SetGlobal("KEY_NUMPAD_PLUS", new Variant(KEY_NUMPAD_PLUS));
    scriptingSystem->SetGlobal("KEY_NUMPAD_0", new Variant(KEY_NUMPAD_0));
    scriptingSystem->SetGlobal("KEY_NUMPAD_1", new Variant(KEY_NUMPAD_1));
    scriptingSystem->SetGlobal("KEY_NUMPAD_2", new Variant(KEY_NUMPAD_2));
    scriptingSystem->SetGlobal("KEY_NUMPAD_3", new Variant(KEY_NUMPAD_3));
    scriptingSystem->SetGlobal("KEY_NUMPAD_4", new Variant(KEY_NUMPAD_4));
    scriptingSystem->SetGlobal("KEY_NUMPAD_5", new Variant(KEY_NUMPAD_5));
    scriptingSystem->SetGlobal("KEY_NUMPAD_6", new Variant(KEY_NUMPAD_6));
    scriptingSystem->SetGlobal("KEY_NUMPAD_7", new Variant(KEY_NUMPAD_7));
    scriptingSystem->SetGlobal("KEY_NUMPAD_8", new Variant(KEY_NUMPAD_8));
    scriptingSystem->SetGlobal("KEY_NUMPAD_9", new Variant(KEY_NUMPAD_9));
    scriptingSystem->SetGlobal("KEY_FUNC_F1", new Variant(KEY_FUNC_F1));
    scriptingSystem->SetGlobal("KEY_FUNC_F2", new Variant(KEY_FUNC_F2));
    scriptingSystem->SetGlobal("KEY_FUNC_F3", new Variant(KEY_FUNC_F3));
    scriptingSystem->SetGlobal("KEY_FUNC_F4", new Variant(KEY_FUNC_F4));
    scriptingSystem->SetGlobal("KEY_FUNC_F5", new Variant(KEY_FUNC_F5));
    scriptingSystem->SetGlobal("KEY_FUNC_F6", new Variant(KEY_FUNC_F6));
    scriptingSystem->SetGlobal("KEY_FUNC_F7", new Variant(KEY_FUNC_F7));
    scriptingSystem->SetGlobal("KEY_FUNC_F8", new Variant(KEY_FUNC_F8));
    scriptingSystem->SetGlobal("KEY_FUNC_F9", new Variant(KEY_FUNC_F9));
    scriptingSystem->SetGlobal("KEY_FUNC_F10", new Variant(KEY_FUNC_F10));
    scriptingSystem->SetGlobal("KEY_FUNC_F11", new Variant(KEY_FUNC_F11));
    scriptingSystem->SetGlobal("KEY_FUNC_F12", new Variant(KEY_FUNC_F12));
    
    // Collision notifications
    scriptingSystem->SetGlobal("COLLISION_START", new Variant(PhysicsCollision::COLLISION_START));
    scriptingSystem->SetGlobal("COLLISION_END", new Variant(PhysicsCollision::COLLISION_END));

	/* Friendly names
	DataRecordType* transformType = DataRecordType::GetType("Transform");
	transformType->SetKeyFriendlyName("m_root", "Root");
	transformType->SetKeyFriendlyName("m_position", "Position");
	transformType->SetKeyFriendlyName("m_rotation", "Rotation");
	transformType->SetKeyFriendlyName("m_scaling", "Scaling");

	DataRecordType* resourceType = DataRecordType::GetType("Resource");
	resourceType->SetKeyFriendlyName("filename", "File");*/
}

void Application::Shutdown() {
	
}

Application* Application::GetInstance() {
	if (m_instance == 0) {
		m_instance = new Application();
	}

	return(m_instance);
}

World* Application::GetWorld() {
	if (m_world == 0) {
		m_world = new World();
	}

	return(m_world);
}
