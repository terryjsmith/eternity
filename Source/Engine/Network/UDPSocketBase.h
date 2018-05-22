
#ifndef udpsocketbase_hpp
#define udpsocketbase_hpp

#ifdef _WIN32
#include <WinSock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#endif

#include <eternity.h>

/**
 * UDP socket base class
 */
class GIGA_API UDPSocketBase {
public:
    UDPSocketBase() : m_socket(0) { }
    virtual ~UDPSocketBase() { }
    
public:
    /**
     * Connect to the server
     */
    virtual bool Connect(std::string server, int port) = 0;
    
    /**
     * Send a message
     */
    virtual int Write(unsigned char* buffer, int length) = 0;
    
    /**
     * Receive a message
     */
    virtual int Read(void* buffer, int length) = 0;
    
    /**
     * Close the connection to the server
     */
    virtual void Close() = 0;
    
    /**
     * Get the raw socket
     */
    int GetSocket() { return m_socket; }
    void SetSocket(int socket) { m_socket = socket; }
    
    /**
     * Create a listening socket
     */
    virtual void Listen(int port) { }
    
    /**
     * Accept a new socket connection
     */
    virtual void Accept(int socket) { }
    
    /**
     * Set the destination address
     */
    virtual void SetSocketAddress(sockaddr_in* sock, socklen_t length) { }
    
    /**
     * Get the last known/read socket address
     */
    virtual sockaddr_in* GetLastSocketAddress(socklen_t& len) { return(0); }
    
protected:
    // Our system socket
    int m_socket;
    
    // The server we are connected to
    std::string m_server;
    
    // The port we are connected to
    int m_port;
    
    // Whether we are connected (have done the handshake or not)
    bool m_connected;
    
    sockaddr_in m_lastsockaddr;
    socklen_t m_lastLength;
    
    // Socket descriptor
    struct sockaddr_in m_sockaddr;
    
    // Length of the socket descriptor
    unsigned int m_addrlen;
};

#endif
