
#include <Network/Messages/EchoRequestMessage.h>
#include <Core/TimeSystem.h>
#include <Network/NetworkSystem.h>
#include <Network/ReplicationSystem.h>
#include <Core/Application.h>
#include <Network/Messages/NetworkMessageTypes.h>
#include <Network/Messages/EchoResponseMessage.h>

void EchoRequestMessage::OnSend() {
    // Send the current timestamp of our client so we can receive it back in a connect reply and get RTT
    m_envelope.type = NetworkMessages::idEchoRequestMessage;
    
    // Get the current client time
    timespec currentTime;
    TimeSystem::GetTimestamp(&currentTime);
    
    uint32_t currentSeconds = (uint32_t)currentTime.tv_sec;
    uint32_t currentNsecond = (uint32_t)currentTime.tv_nsec;
    
    // Pack the current time
    unsigned char* message = (unsigned char*)malloc(sizeof(uint32_t) * 4);
    
    int offset = 0;
    memcpy(message + offset, &currentSeconds, sizeof(uint32_t));
    
    offset += sizeof(uint32_t);
    memcpy(message + offset, &currentNsecond, sizeof(uint32_t));
    
    NetworkSystem* networkSystem = GetSystem<NetworkSystem>();
    NetworkSession* session = networkSystem->FindSession(0);
    offset += sizeof(uint32_t);
    
    uint32_t pingTime = floor(session->info.pingTime * 1000);
    memcpy(message + offset, &pingTime, sizeof(uint32_t));
    
    offset += sizeof(uint32_t);
    uint32_t offsetTime = floor(session->info.clientTimeDiff * 1000);
    memcpy(message + offset, &offsetTime, sizeof(uint32_t));
    
    SetPayload(message, sizeof(uint32_t) * 4);
}

void EchoRequestMessage::OnReceive() {
    // Receive the local timestamp from a client and echo it back along with our server start time
    
    // Read out the current timestamp of the client
    uint32_t clientSeconds, clientNsecond;
    uint32_t clientPing, clientOffset;
    
    ReadPayload(&clientSeconds, sizeof(uint32_t));
    ReadPayload(&clientNsecond, sizeof(uint32_t));
    ReadPayload(&clientPing, sizeof(uint32_t));
    ReadPayload(&clientOffset, sizeof(uint32_t));
    
    timespec ts;
    ts.tv_sec = clientSeconds;
    ts.tv_nsec = clientNsecond;
    
    // Create a reply
    EchoResponseMessage* response = new EchoResponseMessage();
    response->SetClientTimestamp(ts);
    
    // Server will send this back
    NetworkSystem* networkSystem = GetSystem<NetworkSystem>();
    networkSystem->Send(m_envelope.session, response);
    
    // Update last ping time for session
    NetworkSession* session = networkSystem->FindSession(m_envelope.session);
    session->lastPing = networkSystem->GetCurrentTick();
    
    float pingTime = (float)clientPing / 1000.0f;
    session->info.pingTimes[session->info.pingIndex] = pingTime;
    session->info.pingIndex = (session->info.pingIndex + 1) % 10;
    
    // Determine average ping time
    float avgPingTime = 0.0f;
    int counter = 0;
    for(int i = 0; i < 10; i++) {
        if (session->info.pingTimes[i] > 0) {
            avgPingTime += session->info.pingTimes[i];
            counter++;
        }
    }
    avgPingTime /= counter;
    session->info.pingTime = avgPingTime;
    
    float offsetValue = (float)clientOffset / 1000.0f;
    session->info.offsetTimes[session->info.offsetIndex] = offsetValue;
    session->info.offsetIndex = (session->info.offsetIndex + 1) % 10;
    
    // Determine average client offset
    float avgClientOffset = 0.0f;
    counter = 0;
    for (int i = 0; i < 10; i++) {
        if (session->info.offsetTimes[i] > 0) {
            avgClientOffset += session->info.offsetTimes[i];
            counter++;
        }
    }
    
    avgClientOffset /= counter;
    session->info.clientTimeDiff = avgClientOffset;
    
    /*if (session->info.clientTimeDiff == 0) {
     session->info.clientTimeDiff = (float)clientOffset / 1000.0f;
     }*/
    
    printf("Client ping time: %d ms (%f avg), offset %d ms\n", clientPing, avgPingTime * 1000.0f, avgClientOffset * 1000.0f);
    
    // Queue up a full snapshot
    ReplicationSystem* replicationSystem = GetSystem<ReplicationSystem>();
    replicationSystem->SendFullSnapshot(m_envelope.session);
}
