
#include <Network/Messages/EchoResponseMessage.h>
#include <Core/TimeSystem.h>
#include <Network/NetworkSystem.h>
#include <Core/Application.h>
#include <Network/Messages/NetworkMessageTypes.h>

void EchoResponseMessage::OnSend() {
    // Send the server start time + current server time
    m_envelope.type = NetworkMessages::idEchoResponseMessage;
    
    // Get the current server time
    timespec currentTime;
    TimeSystem::GetTimestamp(&currentTime);
    
    uint32_t currentSeconds = (uint32_t)currentTime.tv_sec;
    uint32_t currentNsecond = (uint32_t)currentTime.tv_nsec;
    
    // Get the server start time
    NetworkSystem* networkSystem = GetSystem<NetworkSystem>();
    timespec startTime = networkSystem->GetStartupTime();
    
    uint32_t startSeconds = (uint32_t)startTime.tv_sec;
    uint32_t startNsecond = (uint32_t)startTime.tv_nsec;
    
    // Finally, get the timestamp the client sent to us
    uint32_t pingSeconds = (uint32_t)m_clientTime.tv_sec;
    uint32_t pingNsecond = (uint32_t)m_clientTime.tv_nsec;
    
    // Pack the current time + start time + current tick
    unsigned char* message = (unsigned char*)malloc(sizeof(uint32_t) * 6);
    
    int offset = 0;
    memcpy(message + offset, &startSeconds, sizeof(uint32_t));
    
    offset += sizeof(uint32_t);
    memcpy(message + offset, &startNsecond, sizeof(uint32_t));
    
    offset += sizeof(uint32_t);
    memcpy(message + offset, &currentSeconds, sizeof(uint32_t));
    
    offset += sizeof(uint32_t);
    memcpy(message + offset, &currentNsecond, sizeof(uint32_t));
    
    offset += sizeof(uint32_t);
    memcpy(message + offset, &pingSeconds, sizeof(uint32_t));
    
    offset += sizeof(uint32_t);
    memcpy(message + offset, &pingNsecond, sizeof(uint32_t));
    
    SetPayload(message, sizeof(uint32_t) * 6);
}

void EchoResponseMessage::OnReceive() {
    // Receive back the start time of the server, current server time + original client timestamp
    uint32_t startSeconds, startNsecond;
    uint32_t currentSeconds, currentNsecond;
    uint32_t pingSeconds, pingNsecond;
    
    ReadPayload(&startSeconds, sizeof(uint32_t));
    ReadPayload(&startNsecond, sizeof(uint32_t));
    ReadPayload(&currentSeconds, sizeof(uint32_t));
    ReadPayload(&currentNsecond, sizeof(uint32_t));
    ReadPayload(&pingSeconds, sizeof(uint32_t));
    ReadPayload(&pingNsecond, sizeof(uint32_t));
    
    // First, figure out the round trip time (RTT) from client timestamp
    timespec now;
    TimeSystem::GetTimestamp(&now);
    
    timespec ping;
    ping.tv_sec = pingSeconds;
    ping.tv_nsec = pingNsecond;
    
    timespec delta = TimeSystem::Diff(&ping, &now);
    float rtt = (float)delta.tv_sec + ((float)delta.tv_nsec / 1000000000.0f);
    
    // Get our session
    NetworkSystem* networkSystem = GetSystem<NetworkSystem>();
    NetworkSession* session = networkSystem->FindSession(m_envelope.session);
    
    float pingTime = rtt / 2.0f;
    session->info.pingTimes[session->info.pingIndex] = pingTime;
    session->info.pingIndex = (session->info.pingIndex + 1) % 10;
    
    // Determine average ping time
    float avgPingTime = 0.0f;
    int counter = 0;
    for (int i = 0; i < 10; i++) {
        if (session->info.pingTimes[i] != 0) {
            avgPingTime += session->info.pingTimes[i];
            counter++;
        }
    }
    avgPingTime /= counter;
    session->info.pingTime = avgPingTime;
    
    // Update properties (round trip time / 2.0)
    // session->info.pingTime = rtt / 2.0f;
    
    // Use the ping time to figure out the client offset
    timespec serverTime;
    serverTime.tv_sec = currentSeconds;
    serverTime.tv_nsec = currentNsecond;
    
    // Get the time difference from current server time to us
    timespec offset = TimeSystem::Diff(&serverTime, &now);
    float diff = (float)offset.tv_sec + ((float)offset.tv_nsec / 1000000000.0f);
    
    float offsetValue = diff - session->info.pingTime;
    session->info.offsetTimes[session->info.offsetIndex] = offsetValue;
    session->info.offsetIndex = (session->info.offsetIndex + 1) % 10;
    
    // Determine average client offset
    float avgClientOffset = 0.0f;
    counter = 0;
    for (int i = 0; i < 10; i++) {
        if (session->info.offsetTimes[i] != 0) {
            avgClientOffset += session->info.offsetTimes[i];
            counter++;
        }
    }
    
    avgClientOffset /= counter;
    session->info.clientTimeDiff = avgClientOffset;
    
    /* Minus the ping time
     float d = diff - session->info.pingTime;
     if (session->info.clientTimeDiff == 0) {
     session->info.clientTimeDiff = d;
     }*/
    
    // Save the start time
    timespec startTime;
    startTime.tv_sec = startSeconds;
    startTime.tv_nsec = startNsecond;
    
    networkSystem->SetStartupTime(startTime);
}
