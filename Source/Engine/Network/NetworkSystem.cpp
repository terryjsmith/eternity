
#include <Network/NetworkSystem.h>
#include <Core/TimeSystem.h>
#include <Core/Error.h>
#include <Network/Messages/AckMessage.h>
#include <Network/NetworkMessagePart.h>
#include <Network/Messages/ResendPartialMessage.h>
#include <Network/Messages/EchoRequestMessage.h>
#include <Network/ReplicationSystem.h>
#include <Core/Application.h>

NetworkSystem::NetworkSystem() {
    m_systemType = 0;
    m_lastMessageID = 0;
    m_maxSocketID = 0;
    memset(&m_startupTime, 0, sizeof(timespec));
    m_overrideTick = 0;
}

NetworkSystem::~NetworkSystem() {
    
}

void NetworkSystem::Initialize() {
    TimeSystem::GetTimestamp(&m_startupTime);
}

void NetworkSystem::Connect(std::string host, int port) {
    // Set as client type
    m_systemType = NETWORK_SYSTEM_CLIENT;
    
    // Setup client info
    NetworkClientInfo* info = new NetworkClientInfo();
    info->timeOffset = {0, 0};
    FD_ZERO(&info->sockets);
    
    // Create a new network session
    NetworkSession* session = new NetworkSession();
    
    // Create socket
    session->socket = new UDPSocket();
    if(session->socket->Connect(host, port) == false) {
        delete session;
        delete info;
        
        return;
    }
    
    // Save session info
    info->session = session;
    
    // Save
    m_info.client_info = info;
    
    // Set max socket ID for polling
    m_maxSocketID = std::max(m_maxSocketID, session->socket->GetSocket());
    FD_SET(session->socket->GetSocket(), &info->sockets);
    
    /* Send first echo message request
     EchoRequestMessage* request = new EchoRequestMessage();
     Send(request);
     delete request;*/
}

void NetworkSystem::Listen(int port) {
    // Set as server type
    m_systemType = NETWORK_SYSTEM_SERVER;
    
    // Set up server info
    NetworkServerInfo* info = new NetworkServerInfo();
    FD_ZERO(&info->listenSocket);
    
    // Create a new socket and listen on it
    info->socket = new UDPSocket();
    info->socket->Listen(port);
    
    // Initialize our socket listener lists
    FD_SET(info->socket->GetSocket(), &info->listenSocket);
    
    // Save
    m_info.server_info = info;
    
    // Set max socket ID for polling
    m_maxSocketID = std::max(m_maxSocketID, info->socket->GetSocket());
}

void NetworkSystem::Send(NetworkSession* session, NetworkMessage* msg) {
    if(m_systemType == 0) {
        return;
    }
    
    // Fill in message ID and tick
    NetworkMessage::NetworkEnvelope* env = msg->GetEnvelope();
    env->tick = GetCurrentTick();
    env->id = ++m_lastMessageID;
    
    int written = 0;
    
    if (env->bytes > NETWORK_MAX_PACKET_SIZE) {
        // Populate payload on original message
        msg->OnSend();
        
        // Get the original envelope
        NetworkMessage::NetworkEnvelope* env = msg->GetEnvelope();
        
        // Find the constructor for the message type
        std::map<int, NetworkMessage*(*)()>::iterator it = m_messageTypes.find(env->type);
        
        // Get raw payload
        unsigned char* rawPayload = msg->GetRawPayload();
        
        // Break up into smaller messages
        int totalPackets = ceil((float)env->bytes / NETWORK_MAX_PACKET_SIZE);
        for (int i = 0; i < totalPackets; i++) {
            NetworkMessage* newMsg = it->second();
            NetworkMessage::NetworkEnvelope* newEnv = newMsg->GetEnvelope();
            
            // Get the number of bytes in this packet
            newEnv->chunkID = i + 1;
            int bytes = (newEnv->chunkID * NETWORK_MAX_PACKET_SIZE > env->bytes) ? env->bytes - (newEnv->chunkID * NETWORK_MAX_PACKET_SIZE) : NETWORK_MAX_PACKET_SIZE;
            
            newEnv->tick = env->tick;
            newEnv->id = env->id;
            newEnv->bytes = env->bytes;
            newEnv->end = bytes;
            newEnv->flags = env->flags;
            
            newMsg->SetPayload(rawPayload + (i * NETWORK_MAX_PACKET_SIZE), bytes);
            
            session->Write(newMsg);
            delete newMsg;
        }
        
        return;
    }
    
    session->Write(msg);
    
    // If this message requires an ack response, save a copy
    if (env->flags & NetworkMessage::Flags::FLAG_ACK) {
        std::map<int, NetworkMessage*(*)()>::iterator it = m_messageTypes.find(env->type);
        NetworkMessage* copy = it->second();
        msg->Copy(copy);
        
        m_acks.push_back(copy);
    }
}

void NetworkSystem::Send(NetworkMessage* msg) {
    if(m_systemType == 0) {
        return;
    }
    
    if (m_systemType == NETWORK_SYSTEM_CLIENT) {
        Send(m_info.client_info->session, msg);
    }
    else {
        for (size_t i = 0; i < m_info.server_info->sessions.size(); i++) {
            Send(m_info.server_info->sessions[i], msg);
        }
    }
}

void NetworkSystem::Send(int sessionID, NetworkMessage* msg) {
    if(m_systemType == 0) {
        return;
    }
    
    GIGA_ASSERT(m_systemType == NETWORK_SYSTEM_SERVER, "This function should only be called on servers.");
    
    for(size_t i = 0; i < m_info.server_info->sessions.size(); i++) {
        if(m_info.server_info->sessions[i]->sessionID == sessionID) {
            Send(m_info.server_info->sessions[i], msg);
            return;
        }
    }
    
    GIGA_ASSERT(false, "Session not found to send to.");
}

NetworkSession* NetworkSystem::FindSession(int sessionID, UDPSocket* socket) {
    // If this is the client, just return our default session
    if(m_systemType == NETWORK_SYSTEM_CLIENT) {
        return(m_info.client_info->session);
    }
    
    // Otherwise, try to find our session
    for(size_t i = 0; i < m_info.server_info->sessions.size(); i++) {
        if(m_info.server_info->sessions[i]->sessionID == sessionID) {
            return(m_info.server_info->sessions[i]);
        }
    }
    
    GIGA_ASSERT(socket != 0, "Session not found and socket data not supplied.");
    
    // Add a new session
    NetworkSession* session = new NetworkSession();
    
    // Set socket properties
    session->socket = new UDPSocket();
    
    // Get data for the last socket we read from
    socklen_t len = 0;
    sockaddr_in* in = socket->GetLastSocketAddress(len);
    
    // Set it on new socket for future sending
    session->socket->SetSocketAddress(in, len);
    session->sessionID = sessionID;
    session->socket->SetSocket(socket->GetSocket());
    
    // Add to list
    m_info.server_info->sessions.push_back(session);
    
    Message::Broadcast(new Error(Error::ERROR_DEBUG, "New session", std::to_string(sessionID)));
    
    // Queue up the session for a full sync
    ReplicationSystem* replicationSystem = GetSystem<ReplicationSystem>();
    replicationSystem->SendFullSnapshot(sessionID);
    
    return(session);
}

void NetworkSystem::RemoveSession(int sessionID) {
    if(m_systemType == NETWORK_SYSTEM_CLIENT) {
        delete m_info.client_info->session;
        m_info.client_info->session = 0;
    }
    
    std::vector<NetworkSession*>::iterator i = m_info.server_info->sessions.begin();
    for(i; i != m_info.server_info->sessions.end(); i++) {
        if((*i)->sessionID == sessionID) {
            delete (*i);
            m_info.server_info->sessions.erase(i);
            
            return;
        }
    }
}

void NetworkSystem::Update(float delta) {
    // Make sure we're initialized and have something to update
    if(m_systemType == 0) {
        return;
    }
    
    // Get current tick
    int tick = GetCurrentTick();
    
    // Check if there is any data waiting
    timeval wait = {0, 0};
    
    fd_set readfds;
    UDPSocket* socket = 0;
    if(m_systemType == NETWORK_SYSTEM_CLIENT) {
        readfds = m_info.client_info->sockets;
        socket = m_info.client_info->session->socket;
    }
    else {
        readfds = m_info.server_info->listenSocket;
        socket = m_info.server_info->socket;
    }
    
    if(select(m_maxSocketID + 1, &readfds, 0, 0, &wait)) {
        // Create a buffer big enough to hold a large-ish packet
        unsigned char* buffer = (unsigned char*)malloc(NETWORK_MAX_PACKET_SIZE);
        int bytes = socket->Read(buffer, NETWORK_MAX_PACKET_SIZE);
        
        while(bytes > 0) {
            // Read in the message type (first thing in envelope)
            uint16_t type = 0;
            memcpy(&type, buffer, sizeof(uint16_t));
            
            NetworkMessage* msg = 0;
            std::map<int, NetworkMessage*(*)()>::iterator it = m_messageTypes.find((int)type);
            GIGA_ASSERT(it != m_messageTypes.end(), "Unregistered message type.");
            
            // Call the constructor for the specific class
            msg = it->second();
            msg->Initialize(buffer, bytes);
            
            // If we're in server mode, check to see if this is a new session
            NetworkMessage::NetworkEnvelope* env = msg->GetEnvelope();
            NetworkSession* session = FindSession(env->session, socket);
            
            if(env->bytes == env->end && env->chunkID == 1) {
                // If this message is a full message, just parse it
                msg->OnReceive();
                
                // If we need an ack, send it
                if (env->flags & NetworkMessage::Flags::FLAG_ACK) {
                    AckMessage* ack = new AckMessage();
                    ack->messageID = env->id;
                    Send(session, ack);
                    
                    printf("Sending ack for message ID %d on session ID %d.\n", env->id, env->session);
                    
                    delete ack;
                }
                delete msg;
            }
            else {
                // If this packet is a partial, store it to be actioned later
                std::map<uint32_t, NetworkMessagePart*>::iterator i = m_partials.find(env->id);
                if (i == m_partials.end()) {
                    NetworkMessagePart* part = new NetworkMessagePart();
                    part->master = msg;
                    part->parts.push_back(msg);
                    part->lastTick = tick;
                    part->session = session;
                    
                    // Store this message
                    m_partials[env->id] = part;
                }
                else {
                    // If this is for a packet we've already processed, move on
                    if ((*i).second->processed == true) {
                        delete msg;
                    }
                    else {
                        // Append it to the existing message and discard
                        uint32_t envelopeID = env->id;
                        m_partials[envelopeID]->master->Append(buffer, env->chunkID * NETWORK_MAX_PACKET_SIZE, env->bytes);
                        
                        // Note timing
                        m_partials[envelopeID]->lastTick = tick;
                        
                        // Check again for a complete packet
                        NetworkMessage::NetworkEnvelope* env = m_partials[envelopeID]->master->GetEnvelope();
                        if (env->bytes == env->end) {
                            m_partials[envelopeID]->master->OnReceive();
                            m_partials[envelopeID]->processed = true;
                        }
                    }
                }
            }
            
            // Get the next message
            bytes = socket->Read(buffer, NETWORK_MAX_PACKET_SIZE);
        }
        
        if(bytes == 0) {
            // TODO: Socket closed, should be removed
        }
        
        // Free buffer data
        free(buffer);
    }
    
    // TODO: Send ack packets
    
    // Re-request missing partial messages or ack requests
    ResendPartialMessage* resendMsg = new ResendPartialMessage();
    bool sendResend = false;
    std::map<uint32_t, NetworkMessagePart*>::iterator i = m_partials.begin();
    for(; i != m_partials.end(); i++) {
        NetworkMessagePart* part = (*i).second;
        
        // Figure out latency using RTT
        float latency = part->session->info.pingTime * 2;
        float diff = (tick - part->lastTick) / NETWORK_TICKS_PER_SECOND;
        
        // If we have gone past twice our average latency, re-request pieces
        NetworkMessage::NetworkEnvelope* env = part->master->GetEnvelope();
        
        if(diff > latency && part->processed == false) {
            // Determine total chunks
            int totalChunks = ceil((float)env->bytes / NETWORK_MAX_PACKET_SIZE);
            
            // Map which ones have been received
            std::map<int, bool> received;
            for(int j = 1; j <= totalChunks; j++) {
                received[j] = false;
            }
            
            // Check which have been received
            std::vector<NetworkMessage*>::iterator m = part->parts.begin();
            for(; m != part->parts.end(); m++) {
                received[((*m)->GetEnvelope())->chunkID] = true;
            }
            
            // Add missing items
            std::map<int, bool>::iterator r = received.begin();
            for(; r != received.end(); r++) {
                if(r->second == false) {
                    resendMsg->AddPacket(env->id, r->first);
                    sendResend = true;
                }
            }
        }
        
        // Delete any partials that have already been processed afer a certain time
        if(part->lastTick < (tick - NETWORK_RESEND_HISTORY) && part->processed == true) {
            int envelopeID = (*i).first;
            m_partials.erase(envelopeID);
            i--;
            
            delete part;
        }
    }
    
    if(sendResend) {
        Send(resendMsg);
    }
    
    delete resendMsg;
    
    // Send echo packet (if client)
    if(m_systemType == NETWORK_SYSTEM_CLIENT) {
        if(m_info.client_info->session->lastPing < (tick - NETWORK_ECHO_TICKS)) {
            EchoRequestMessage* request = new EchoRequestMessage();
            Send(request);
            
            m_info.client_info->session->lastPing = tick;
            delete request;
        }
    }
    
    if(m_systemType == NETWORK_SYSTEM_SERVER) {
        // Check for timed out clients
        for(size_t i = 0; i < m_info.server_info->sessions.size(); i++) {
            if(m_info.server_info->sessions[i]->lastPing < (tick - NETWORK_ECHO_TIMEOUT) && m_info.server_info->sessions[i]->lastPing > 0) {
                printf("Last ping: %d, current tick: %d\n", m_info.server_info->sessions[i]->lastPing, tick);
                Message::Broadcast(new Error(Error::ERROR_DEBUG, "Disconnecting session", std::to_string(m_info.server_info->sessions[i]->sessionID)));
                RemoveSession(m_info.server_info->sessions[i]->sessionID);
            }
        }
    }
    
    // Re-request any ack packets
}

int NetworkSystem::GetCurrentTick() {
    if (m_overrideTick) {
        return(m_overrideTick);
    }
    
    timespec t;
    TimeSystem::GetTimestamp(&t);
    
    timespec diff = TimeSystem::Diff(&m_startupTime, &t);
    double d = (double)diff.tv_sec + ((double)diff.tv_nsec / 1000000000);
    
    // If this is the client, add the offset in
    if(m_systemType == NETWORK_SYSTEM_CLIENT) {
        d -= m_info.client_info->session->info.clientTimeDiff;
    }
    
    return(floor(d * NETWORK_TICKS_PER_SECOND));
}

float NetworkSystem::GetCurrentTickTime() {
    timespec t;
    TimeSystem::GetTimestamp(&t);
    
    timespec diff = TimeSystem::Diff(&m_startupTime, &t);
    double d = (double)diff.tv_sec + ((double)diff.tv_nsec / 1000000000);
    
    // If this is the client, add the offset in
    if(m_systemType == NETWORK_SYSTEM_CLIENT) {
        d -= m_info.client_info->session->info.clientTimeDiff;
    }
    
    return(d);
}

void NetworkSystem::MarkReceived(int sessionID, int messageID) {
    std::vector<NetworkMessage*>::iterator i = m_acks.begin();
    for (; i != m_acks.end(); i++) {
        NetworkMessage::NetworkEnvelope* env = (*i)->GetEnvelope();
        if (env->session == sessionID && env->id == messageID) {
            (*i)->OnAck();
            
            delete (*i);
            m_acks.erase(i);
            
            return;
        }
    }
}
