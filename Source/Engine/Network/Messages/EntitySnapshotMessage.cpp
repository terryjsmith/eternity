
#include <Network/Messages/EntitySnapshotMessage.h>
#include <Network/EntitySnapshot.h>
#include <Network/ReplicationSystem.h>
#include <Network/NetworkSystem.h>
#include <Core/Application.h>
#include <Network/Messages/NetworkMessageTypes.h>

void EntitySnapshotMessage::SetEntityPayload(unsigned char* payload, int size) {
    SetPayload(payload, size);
}

void EntitySnapshotMessage::OnSend() {
    // Nothing to put here, payload set by SetEntityPayload
    m_envelope.type = NetworkMessages::idEntitySnapshotMessage;
}

void EntitySnapshotMessage::OnReceive() {
    EntitySnapshot* snapshot = new EntitySnapshot();
    snapshot->tick = m_envelope.tick;
    snapshot->SetData(m_payload, m_envelope.bytes);
    snapshot->Deserialize();
    
    ReplicationSystem* replicationSystem = GetSystem<ReplicationSystem>();
    replicationSystem->AddSnapshot(m_envelope.tick, snapshot);
    
    // Check if we can make the server authoritative again
    NetworkSystem* networkSystem = GetSystem<NetworkSystem>();
    NetworkSession* session = networkSystem->FindSession(0);
    if (m_envelope.lastCmd > session->lastCommandMessage) {
        replicationSystem->SetClientAuthoritative(false);
        session->lastCommandMessage = m_envelope.lastCmd;
    }
}
