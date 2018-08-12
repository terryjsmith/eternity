
#ifndef WIN32

#include <Network/UDPSocketPosix.h>
#include <Core/Error.h>

bool UDPSocketPosix::Connect(std::string server, int port) {
    m_server = server;
    m_port = port;
    
    // Create a socket
    m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(m_socket < 0) {
        Message::Broadcast(new Error(Error::ERROR_WARN, "Unable to create socket for server", server));
        return(false);
    }
    
    // Get the host IP in a computer usable home
    struct hostent* host;
    host = gethostbyname(server.c_str());
    if(host == 0) {
        Message::Broadcast(new Error(Error::ERROR_WARN, "Unable to get host", server));
        return(false);
    }
    
    int flags = fcntl(m_socket, F_GETFL, 0);
    if (flags < 0) return false;
    flags = flags|O_NONBLOCK;
    if(fcntl(m_socket, F_SETFL, flags) != 0) {
        Message::Broadcast(new Error(Error::ERROR_WARN, "Unable to set non-blocking connection type.", server));
        return(false);
    }
    
    // Create our server structure
    m_sockaddr.sin_family = AF_INET;
    bcopy((char *)host->h_addr, (char *)&m_sockaddr.sin_addr.s_addr, host->h_length);
    m_sockaddr.sin_port = htons(port);
    
    // Connect
    if(::connect(m_socket, (struct sockaddr*)&m_sockaddr, sizeof(m_sockaddr)) < 0) {
        Message::Broadcast(new Error(Error::ERROR_WARN, "Unable to connect to host", server));
        return(false);
    }
    
    m_addrlen = sizeof(m_sockaddr);
    m_connected = true;
    
    return(true);
}

int UDPSocketPosix::Write(unsigned char* buffer, int length) {
    socklen_t len = sizeof(m_sockaddr);
    int ret = 0;
    
    if(m_connected) {
        ret = (int)send(m_socket, (void*)buffer, length, 0);
    }
    else {
        ret = (int)sendto(m_socket, (void*)buffer, length, 0, (struct sockaddr*)&m_sockaddr, len);
    }
    
    int err = errno;
    return(ret);
}

int UDPSocketPosix::Read(void* buffer, int length) {
    int ret = 0;
    socklen_t len = sizeof(sockaddr_in);
    
    if(m_connected) {
        ret = (int)recv(m_socket, buffer, length, MSG_DONTWAIT);
    }
    else {
        ret = (int)recvfrom(m_socket, buffer, length, MSG_DONTWAIT, (struct sockaddr*)&m_lastsockaddr, &len);
        m_lastLength = len;
    }
    
    return(ret);
}

void UDPSocketPosix::Close() {
    if(m_socket) {
        close(m_socket);
        m_socket = 0;
    }
}

void UDPSocketPosix::Listen(int port) {
    m_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if(m_socket < 0) {
        Message::Broadcast(new Error(Error::ERROR_WARN, "Unable to initialize socket."));
        return;
    }
    
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(sockaddr_in));
    
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = INADDR_ANY;
    
    int enable = 1;
    if (setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        GIGA_ASSERT(false, "Unable to re-use socket address.");
        Message::Broadcast(new Error(Error::ERROR_WARN, "Unable to re-use socket address."));
        return;
    }
    
    if (setsockopt(m_socket, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int)) < 0) {
        GIGA_ASSERT(false, "Unable to re-use socket address.");
        Message::Broadcast(new Error(Error::ERROR_WARN, "Unable to re-use socket address."));
        return;
    }
    
    if(bind(m_socket, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        GIGA_ASSERT(false, "Unable to bind socket.");
        Message::Broadcast(new Error(Error::ERROR_WARN, "Unable to bind socket."));
        return;
    }
}

void UDPSocketPosix::Accept(int socket) {
    m_socket = socket;
}

void UDPSocketPosix::SetSocketAddress(sockaddr_in* sock, socklen_t length) {
    m_sockaddr.sin_family = sock->sin_family;
    memcpy((void *)&m_sockaddr.sin_addr, (void*)&sock->sin_addr, sizeof(in_addr));
    m_sockaddr.sin_port = sock->sin_port;
}

sockaddr_in* UDPSocketPosix::GetLastSocketAddress(socklen_t& len) {
    len = m_lastLength;
    return(&m_lastsockaddr);
}

#endif
