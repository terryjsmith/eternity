
#ifndef commandmessage_h
#define commandmessage_h

#include <eternity.h>
#include <Network/NetworkMessage.h>
#include <IO/Command.h>

class GIGA_API CommandMessage : public NetworkMessage {
public:
    CommandMessage();
    ~CommandMessage() = default;
    
    GIGA_CLASS_NAME("CommandMessage");
    
    /**
     * Set command
     */
    void SetCommand(Command* command) { m_command = command; }
    
    /**
     * Send a connection request (client)
     */
    void OnSend();
    
    /**
     * Receive a connection request and reply (server)
     */
    void OnReceive();
    
protected:
    Command* m_command;
};

#endif
