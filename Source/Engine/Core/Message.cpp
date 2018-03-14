
#include <Core/Message.h>
#include <Core/MessageSystem.h>
#include <Core/Application.h>

Message::Message() {
	type = 0;
}

void Message::Broadcast(Message* message, bool synchronized) {
	MessageSystem* messageSystem = GetSystem<MessageSystem>();
	messageSystem->Broadcast(message, synchronized);
}
