
#ifndef networkmessagetypes_h
#define networkmessagetypes_h

enum NetworkMessages {
    idEchoRequestMessage = 10,
    idEchoResponseMessage = 20,
    idEntitySnapshotMessage = 30,
    idCommandMessage = 40,
    idResendPartialMessage = 50,
    idAckMessage = 60
};

#endif
