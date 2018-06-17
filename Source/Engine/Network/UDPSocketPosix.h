
#ifndef WIN32

#ifndef udpsocketposix_hpp
#define udpsocketposix_hpp

#include <eternity.h>
#include <Network/UDPSocketBase.h>

class GIGA_API UDPSocketPosix : public UDPSocketBase {
public:
    UDPSocketPosix() = default;
    ~UDPSocketPosix() = default;
    
    /**
     * Connect to the server
     */
    bool Connect(std::string server, int port);
    
    /**
     * Send a message
     */
    int Write(unsigned char* buffer, int length);
    
    /**
     * Receive a message
     */
    int Read(void* buffer, int length);
    
    /**
     * Close the connection to the server
     */
    void Close();
    
    /**
     * Listen on a port for new "connections"
     */
    void Listen(int port);
    
    /**
     * "Accept" a new connection
     */
    void Accept(int socket);
    
    /**
     * Set the destination address
     */
    void SetSocketAddress(sockaddr_in* sock, socklen_t length);
    
    /**
     * Get the last known/read socket address
     */
    sockaddr_in* GetLastSocketAddress(socklen_t& len);
};

#endif

#endif
