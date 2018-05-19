
#include <Core/MessageSystem.h>

MessageSystem::MessageSystem() {
	m_idCounter = 0;
}

void MessageSystem::Update(float delta) {
	int count = m_messages.size();

	for (int i = 0; i < count; i++) {
		for (std::vector<MessageHandler*>::iterator callback = m_handlers.begin(); callback != m_handlers.end(); callback++) {
			if ((*callback)->type == m_messages[i]->type) {
				(*callback)->cb((*callback)->obj, m_messages[i]);
			}
		}
	}

	// Erase as many messages as we had when we started - sometimes messages can spawn other messages
	m_messages.erase(m_messages.begin(), m_messages.begin() + count);
}

void MessageSystem::Broadcast(Message* msg, bool synchronized) {
	// Ensure the type is set
	if (msg->type == 0) {
		std::map<std::string, uint32_t>::iterator it = m_idMapping.find(msg->GetGigaName());
		GIGA_ASSERT(it != m_idMapping.end(), "Unregistered message type.");

		msg->type = it->second;
	}

	if (synchronized) {
		// Iterate over all listeners and dispatch, no queueing
		for (std::vector<MessageHandler*>::iterator callback = m_handlers.begin(); callback != m_handlers.end(); callback++) {
			if ((*callback)->type == msg->type) {
				(*callback)->cb((*callback)->obj, msg);
			}
		}

		return;
	}

	m_messages.push_back(msg);
}

void MessageSystem::RegisterCallback(GigaObject* obj, std::string type, MessageHandlingCallback cb) {
	// Get the mapping
	std::map<std::string, uint32_t>::iterator it = m_idMapping.find(type);
	GIGA_ASSERT(it != m_idMapping.end(), "Message mapping not found.");

	MessageHandler* handler = new MessageHandler();
	handler->type = it->second;
	handler->cb = cb;
	handler->obj = obj;

	m_handlers.push_back(handler);
}

uint32_t MessageSystem::GetMessageType(std::string name) {
    std::map<std::string, uint32_t>::iterator it = m_idMapping.find(name);
    GIGA_ASSERT(it != m_idMapping.end(), "Message mapping not found.");
    
    return(it->second);
}
