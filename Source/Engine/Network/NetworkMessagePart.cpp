
#include <Network/NetworkMessagePart.h>

NetworkMessagePart::NetworkMessagePart() {
    lastTick = 0;
    master = 0;
    session = 0;
    processed = false;
}

NetworkMessagePart::~NetworkMessagePart() {
    std::vector<NetworkMessage*>::iterator i = parts.begin();
    for(; i != parts.end(); i++) {
        delete (*i);
    }
    
    parts.clear();
}
