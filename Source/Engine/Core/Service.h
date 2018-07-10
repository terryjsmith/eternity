
#ifndef service_hpp
#define service_hpp

#include <eternity.h>

/**
 * A base service class for services such as loaders used by various parts of the game engine
 */
class GIGA_API Service {
public:
    Service() = default;
    virtual ~Service() = default;
};

#endif
